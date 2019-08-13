#pragma once
#include <utility>

#include "threadpool.hh"

template <class Function, typename... Args>
auto ThreadPool::submit(Function&& f, Args&&... args)
{
    auto wi_ptr = std::make_unique<WorkItem<Function, Args...>>(f, args...);
    auto future = wi_ptr->get_future();
    this->sched_.submit(std::move(wi_ptr));

    return future;
}
