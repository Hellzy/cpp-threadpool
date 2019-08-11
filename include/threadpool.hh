#pragma once

#define POOL_SIZE 5

#include <array>
#include <vector>
#include <future>

#include "work_item.hh"
#include "work_item_scheduler.hh"

class ThreadPool
{
public:
    ThreadPool() = default;
    ~ThreadPool() = default;

    template <class Function, typename... Args>
    void submit(Function&& f, Args&&... args);

private:
    std::vector<WorkItemPtr> work_;
    WorkItemScheduler sched_;
};

#include "impl/threadpool.hxx"
