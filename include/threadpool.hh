#pragma once

#define POOL_SIZE 5

#include <array>

#include "work_item.hh"
#include "work_item_scheduler.hh"

class ThreadPool
{
public:
    ThreadPool(bool start = true);
    ~ThreadPool() = default;

    void start();
    void stop();

    template <class Function, typename... Args>
    void submit(Function&& f, Args&&... args);

private:
    WorkItemScheduler sched_;
};

#include "impl/threadpool.hxx"
