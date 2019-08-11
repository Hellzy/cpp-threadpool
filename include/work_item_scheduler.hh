#pragma once
#include <cstdint>
#include <vector>

#include "work_item.hh"
#include "worker.hh"

/*
 * Scheduler class responsible for scheduling workers.
 * When a worker is done and needs more work, the scheduler
 * interracts with the theadpool and retrieves one or several
 * work items for a worker
 */
class WorkItemScheduler
{
public:
    WorkItemScheduler(size_t workers_nb = 5);
    ~WorkItemScheduler();

    void start();
    void stop();
    void submit(WorkItemPtr&& wi_ptr);

private:
    void work_dispatch();

private:
    std::vector<Worker> workers_;
    /* Array of eventfds to communicate with workers */
    std::vector<uint64_t> workers_fds_;
    std::vector<WorkItemPtr> work_;
    std::thread dispatcher_thread_;
    int epoll_fd_ = -1;
};
