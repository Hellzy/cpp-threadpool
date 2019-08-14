#include <errno.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <system_error>
#include <unistd.h>

#include "events.hh"
#include "work_item_scheduler.hh"

#define SYS_ERROR(errno, msg)\
    std::system_error(std::make_error_code(std::errc(errno)), (msg))

WorkItemScheduler::WorkItemScheduler(size_t workers_nb)
    : workers_(workers_nb)
{
    this->epoll_fd_ = epoll_create1(0);
    if (this->epoll_fd_ == -1)
        throw SYS_ERROR(errno, "Call to epoll_create1(2) failed");

    for (size_t i = 0; i < workers_nb; ++i)
    {
        int socketfds[2] = {0};
        if (socketpair(AF_UNIX, SOCK_SEQPACKET, 0, socketfds) == -1)
            throw SYS_ERROR(errno, "Call to socketpair(2) failed");

        struct epoll_event ev = {0};
        ev.events = EPOLLIN;
        ev.data.fd = socketfds[0];

        if (epoll_ctl(this->epoll_fd_, EPOLL_CTL_ADD, socketfds[0], &ev) == -1)
        {
            close(socketfds[0]);
            close(socketfds[1]);
            throw SYS_ERROR(errno, "Call to epoll_ctl(2) failed");
        }

        this->workers_[i].set_socketfd(socketfds[1]);
        this->fd_worker_map_[socketfds[0]] = i;
    }
}

WorkItemScheduler::~WorkItemScheduler()
{
    this->stop();
    this->dispatcher_thread_.join();

    for (const auto& pair : this->fd_worker_map_)
        close(pair.first);
    close(this->epoll_fd_);

    for (auto& w : this->workers_)
        w.stop();
}

void WorkItemScheduler::start()
{
    this->active_ = true;

    for (auto& w : this->workers_)
        w.start();

    this->dispatcher_thread_ = std::thread(&WorkItemScheduler::work_dispatch,
                                           this);
}

void WorkItemScheduler::stop()
{
    this->active_ = false;
}

void WorkItemScheduler::submit(WorkItemPtr&& wi_ptr)
{
    this->work_.emplace_back(std::move(wi_ptr));
}

void WorkItemScheduler::work_dispatch()
{
    while (this->active_ || !this->work_.empty())
    {
        struct epoll_event ev = {0};
        int rv = 0;

        do
        {
            rv = epoll_wait(this->epoll_fd_, &ev, 1, 100);

            if (rv == -1 && errno != EINTR)
                throw SYS_ERROR(errno, "Call to epoll_wait(2) failed");
        } while (rv == -1);

        uint64_t event = -1;
        do
        {
            rv = read(ev.data.fd, &event, sizeof(event));

            if (rv == -1 && errno != EINTR)
                throw SYS_ERROR(errno, "Call to read(2) failed");
        } while (rv == -1);

        switch (event)
        {
            case Events::WORK_REQ:
                this->send_work(this->fd_worker_map_[ev.data.fd]);
                break;
            default:
                break;
        }
    }
}

void WorkItemScheduler::send_work(size_t worker_idx)
{
    if (this->work_.size() > 0)
    {
        WorkItemPtr wi_ptr = std::move(this->work_.front());
        this->work_.pop_front();
        this->workers_[worker_idx].push_work(std::move(wi_ptr));
    }
}
