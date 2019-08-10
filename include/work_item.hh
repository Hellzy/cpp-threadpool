#pragma once

#include <utility>

#include "util.hh"

/**
 * A work item is an entity that represents work to be submitted
 * in the future. It holds a function and its arguments that are
 * going to be called in the exec() function.
 */
template <class Function, typename... ArgsC>
class WorkItem
{
public:
    WorkItem(Function f, ArgsC... args)
        : f_(f)
        , args_(args...)
    {}

    void exec() { exec(args_); }

private:
    template <typename... Args, int... Idxs>
    void exec(std::tuple<Args...>& args, util::idx<Idxs...>)
    {
        f_(std::get<Idxs>(args)...);
    }

    template <typename... Args>
    void exec(std::tuple<Args...>& args)
    {
        exec(args, util::unroll<sizeof...(Args)>{});
    }

private:
    Function f_;
    std::tuple<ArgsC...> args_;
};