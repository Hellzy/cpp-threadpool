#include "threadpool.hh"

ThreadPool::ThreadPool(bool start)
{
    if (start)
        this->start();
}

void ThreadPool::start()
{
    this->sched_.start();
}

void ThreadPool::stop()
{
    this->sched_.stop();
}
