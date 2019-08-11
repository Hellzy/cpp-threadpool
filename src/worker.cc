#include <iostream>
#include <unistd.h>

#include "events.hh"
#include "worker.hh"

Worker::~Worker()
{
    this->stop();
    close(this->socketfd_);
}

bool Worker::start()
{
    if (socketfd_ != -1)
    {
        this->alive_ = true;
        this->thread = std::thread(&Worker::work, this);
        return true;
    }

    return false;
}

void Worker::stop()
{
    this->alive_ = false;
    if (this->thread.joinable())
        this->thread.join();
}

void Worker::push_work(WorkItemPtr&& wi_ptr)
{
    this->work_.emplace_back(std::move(wi_ptr));
}

void Worker::set_socketfd(int fd)
{
    this->socketfd_ = fd;
}

void Worker::work()
{
    while (this->alive_)
    {
        if (this->work_.size() > 0)
        {
            auto& wi_ptr = work_.front();
            wi_ptr->exec();
            this->work_.pop_front();
        }
        else
        {
            /* Ask scheduler for work */
            uint64_t event = Events::WORK_REQ;
            write(this->socketfd_, &event, sizeof(event));
        }
    }
}
