#include "worker.hh"

void Worker::start()
{
    this->thread = std::thread(&Worker::work, this);
}

void Worker::stop()
{
    this->thread.join();
}

void Worker::push_work(WorkItemPtr&& wi_ptr)
{
    this->work_.emplace_back(std::move(wi_ptr));
}

void Worker::work()
{
    while (alive)
    {
        if (work_.size() > 0)
        {
            auto& wi_ptr = work_.back();
            wi_ptr->exec();
            this->work_.pop_back();
        }
    }
}
