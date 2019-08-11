#include <iostream>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
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
        int socketfds[2] = {0};
        if (socketpair(AF_UNIX, SOCK_SEQPACKET, 0, socketfds) == -1)
            throw 2; //FIXME

        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = socketfds[0];

        if (epoll_ctl(this->epoll_fd_, EPOLL_CTL_ADD, socketfds[0], &ev) == -1)
        {
            close(socketfds[0]);
            close(socketfds[1]);
            throw 2; //FIXME
        }

        this->workers_[i].set_socketfd(socketfds[1]);
        this->fd_worker_map_[socketfds[0]] = i;
    }
}

WorkItemScheduler::~WorkItemScheduler()
{
    for (auto& w : workers_)
        w.stop();
    this->stop();
    close(this->epoll_fd_);
    //TODO: close all sockets stored in map
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
    if (this->dispatcher_thread_.joinable())
        this->dispatcher_thread_.join();

    for (auto& w : this->workers_)
        w.stop();
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
        std::cout << "Sending work to worker " << worker_idx << '\n';
        WorkItemPtr wi_ptr = std::move(this->work_.front());
        this->work_.pop_front();
        this->workers_[worker_idx].push_work(std::move(wi_ptr));
    }
}
