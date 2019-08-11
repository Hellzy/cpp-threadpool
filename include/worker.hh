#pragma once
#include <deque>
#include <thread>
#include <utility>

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
    void set_socketfd(int fd);

private:
    void work();

private:
    std::thread thread;
    std::deque<WorkItemPtr> work_;
    bool alive_ = false;

    /* Socket used for communication w/ scheduler */
    int socketfd_ = -1;
};
