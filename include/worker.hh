#pragma once
#include <thread>
#include <utility>
#include <vector>

#include "work_item.hh"

/*
 * Worker class that represents one thread of execution.
 * A threadpool is basically a pool of workers that are
 * going to ask for work to the threapool. Communication
 * between workers and the threadpool is handled by the
 * scheduler.
 */
class Worker
{
public:
    Worker() = default;
    ~Worker();

    bool start();
    void stop();
    void push_work(WorkItemPtr&& wi_ptr);
    void set_eventfd(int fd);

private:
    void work();

private:
    std::thread thread;
    std::vector<WorkItemPtr> work_;
    bool alive_ = true;

    /* Eventfd used for communication w/ scheduler */
    int eventfd_ = -1;
};
