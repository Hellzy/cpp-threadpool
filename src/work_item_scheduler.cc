#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "events.hh"
#include "work_item_scheduler.hh"

WorkItemScheduler::WorkItemScheduler(size_t workers_nb)
    : workers_(workers_nb)
{
    this->epoll_fd_ = epoll_create1(0);
    if (this->epoll_fd_ == -1)
        throw 2;//FIXME

    for (size_t i = 0; i < workers_nb; ++i)
    {
        int fd = eventfd(0, EFD_NONBLOCK);
        if (fd == -1)
            throw 2; //FIXME

        struct epoll_event ev = {0};
        ev.events = EPOLLIN;
        ev.data.fd = fd;

        if (epoll_ctl(this->epoll_fd_, EPOLL_CTL_ADD, fd, &ev) == -1)
        {
            close(fd);
            throw 2; //FIXME
        }

        this->workers_[i].set_eventfd(fd);
        this->fd_worker_map_[fd] = i;
    }
}

WorkItemScheduler::~WorkItemScheduler()
{
    for (auto& w : workers_)
        w.stop();
    this->stop();
}

void WorkItemScheduler::start()
{
    this->active_ = true;
    this->dispatcher_thread_ = std::thread(&WorkItemScheduler::work_dispatch,
                                           this);
}

void WorkItemScheduler::stop()
{
    if (this->dispatcher_thread_.joinable())
        this->dispatcher_thread_.join();

    this->active_ = false;
}

void WorkItemScheduler::submit(WorkItemPtr&& wi_ptr)
{
    this->work_.emplace_back(std::move(wi_ptr));
}

void WorkItemScheduler::work_dispatch()
{
    while (this->active_)
    {
        struct epoll_event ev = {0};
        int rv = epoll_wait(this->epoll_fd_, &ev, 1, 100);

        if (rv == -1)
            throw 2; //FIXME

        uint64_t event = -1;
        if (read(ev.data.fd, &event, sizeof(event)) == -1)
            throw 2; //FIXME

        switch (event)
        {
        case Events::WORK_REQ:
            this->send_work(this->fd_worker_map_[ev.data.fd]);
        }
    }
}

void WorkItemScheduler::send_work(size_t worker_idx)
{
    if (this->work_.size() > 0)
    {
        WorkItemPtr& wi_ptr = this->work_.back();
        this->work_.pop_back();
        this->workers_[worker_idx].push_work(std::move(wi_ptr));
    }
}
