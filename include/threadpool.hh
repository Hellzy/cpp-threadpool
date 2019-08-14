#pragma once

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

    /*
     * Submits a work function to the scheduler
     * Returns a std::future
     */
    template <class Function, typename... Args>
    auto submit(Function&& f, Args&&... args);

private:
    WorkItemScheduler sched_;
};

#include "impl/threadpool.hxx"
