#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "work_item_scheduler.hh"

WorkItemScheduler::WorkItemScheduler(size_t workers_nb)
    : workers_(workers_nb)
    , workers_fds_(workers_nb, -1)
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

        this->workers_fds_[i] = fd;
        this->workers_[i].set_eventfd(fd);
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
    this->dispatcher_thread_ = std::thread(&WorkItemScheduler::work_dispatch,
                                           this);
}

void WorkItemScheduler::stop()
{
    if (this->dispatcher_thread_.joinable())
        this->dispatcher_thread_.join();
}

void WorkItemScheduler::submit(WorkItemPtr&& wi_ptr)
{
    this->work_.emplace_back(std::move(wi_ptr));
}

void WorkItemScheduler::work_dispatch()
{
#if 0
    while (true)
    {
        //epoll loop + dispatch
    }
#endif
}
