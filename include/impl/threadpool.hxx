#pragma once
#include <utility>

#include "threadpool.hh"

template <class Function, typename... Args>
void ThreadPool::submit(Function&& f, Args&&... args)
{
    auto wi_ptr = std::make_unique<WorkItem<Function, Args...>>(f, args...);
    this->sched_.submit(std::move(wi_ptr));
}
