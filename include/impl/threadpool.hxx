#pragma once
#include "threadpool.hh"

template <class Function, typename... Args>
void ThreadPool::submit(Function&& f, Args&&... args)
{
    //TODO: get free thread from pool
    //std::move
}
