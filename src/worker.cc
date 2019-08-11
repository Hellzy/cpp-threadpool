#include "worker.hh"

bool Worker::start()
{
    if (eventfd_ != -1)
    {
        this->thread = std::thread(&Worker::work, this);
        return true;
    }

    return false;
}

void Worker::stop()
{
    this->thread.join();
}

void Worker::push_work(WorkItemPtr&& wi_ptr)
{
    this->work_.emplace_back(std::move(wi_ptr));
}

void Worker::set_eventfd(int fd)
{
    eventfd_ = fd;
}

void Worker::work()
{
    while (this->alive_)
    {
        if (this->work_.size() > 0)
        {
            auto& wi_ptr = work_.back();
            wi_ptr->exec();
            this->work_.pop_back();
        }
    }
}
