#pragma once

#define POOL_SIZE 5

#include <array>
#include <vector>
#include <future>

#include "work_item.hh"

class ThreadPool
{
public:
    ThreadPool();
    ~ThreadPool();

    template <class Function, typename... Args>
    void submit(Function&& f, Args&&... args);

private:
    std::array<std::thread, POOL_SIZE> threads_;
    /* True if available, False otherwise */
    std::array<std::atomic_bool, POOL_SIZE> states_;
    std::vector<WorkItemPtr> work_;
};

#include "impl/threadpool.hxx"
