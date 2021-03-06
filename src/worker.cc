#include <unistd.h>

#include "events.hh"
#include "worker.hh"

Worker::~Worker()
{
    this->stop();
    this->thread.join();
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
    bool need_work = true;

    while (this->alive_ || !this->work_.empty())
    {
        if (!this->work_.empty())
        {
            auto& wi_ptr = work_.front();
            wi_ptr->exec();
            this->work_.pop_front();
            need_work = true;
        }
        else if (need_work)
        {
            /* Ask scheduler for work */
            uint64_t event = Events::WORK_REQ;
            write(this->socketfd_, &event, sizeof(event));
            need_work = false;
        }
    }
}
