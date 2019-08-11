#pragma once
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

    void submit(WorkItemPtr&& wi_ptr);

private:
    std::vector<Worker> workers_;
    std::vector<WorkItemPtr> work_;
};
