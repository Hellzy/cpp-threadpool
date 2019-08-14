#pragma once
#include <cstdint>
#include <deque>
#include <map>
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
    WorkItemScheduler(size_t workers_nb =
            std::thread::hardware_concurrency() / 2);
    ~WorkItemScheduler();

    void start();
    void stop();
    void submit(WorkItemPtr&& wi_ptr);

private:
    void work_dispatch();
    void send_work(size_t worker_idx);

private:
    std::vector<Worker> workers_;
    std::deque<WorkItemPtr> work_;
    std::thread dispatcher_thread_;
    int epoll_fd_ = -1;
    bool active_ = false;

    /* A map to keep track of which socket fd belongs to which worker */
    std::map<int, size_t> fd_worker_map_;
};
