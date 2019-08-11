#include "work_item_scheduler.hh"

WorkItemScheduler::WorkItemScheduler(size_t workers_nb)
    : workers_(workers_nb)
{}

WorkItemScheduler::~WorkItemScheduler()
{
    for (auto& w : workers_)
        w.stop();
}

void WorkItemScheduler::submit(WorkItemPtr&& wi_ptr)
{
    this->work_.emplace_back(std::move(wi_ptr));
}
