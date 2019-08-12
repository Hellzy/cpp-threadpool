#pragma once

#include <type_traits>
#include <future>
#include <memory>
#include <utility>

#include "util.hh"

class AbstractWorkItem
{
public:
    virtual ~AbstractWorkItem() {};
    virtual void exec() = 0;
};

using WorkItemPtr = std::unique_ptr<AbstractWorkItem>;

/**
 * A work item is an entity that represents work to be submitted
 * in the future. It holds a function and its arguments that are
 * going to be called in the exec() function.
 */
template <class Function, typename... ArgsC>
class WorkItem : public AbstractWorkItem
{
public:
    using RawFType = typename std::remove_reference<typename std::remove_pointer<Function>::type>::type;

    WorkItem(Function f, ArgsC... args)
        : args_(args...)
        , task(f)
    {}
    ~WorkItem() = default;

    void exec() override { exec(args_); }

private:
    template <typename... Args, int... Idxs>
    void exec(std::tuple<Args...>& args, util::idx<Idxs...>)
    {
        task(std::get<Idxs>(args)...);
    }

    template <typename... Args>
    void exec(std::tuple<Args...>& args)
    {
        exec(args, util::unroll<sizeof...(Args)>{});
    }

private:
    std::tuple<ArgsC...> args_;
    std::packaged_task<RawFType> task;
};
