#pragma once

#include <future>
#include <type_traits>

namespace util
{
/**
 * Next 3 structures are used to unroll parameter packs
 */
template <int... Idxs>
struct idx {};

template <int N, int... Idxs>
struct unroll : unroll<N - 1, N - 1, Idxs...> {};

template <int... Idxs>
struct unroll<0, Idxs...> : idx<Idxs...> {};

/* Used to deduct functions raw types and future types */
template <class F>
struct FType
{
private:
    using UnrefT = typename std::remove_reference<F>::type;
public:
    using RawT = typename std::remove_pointer<UnrefT>::type;
    using FutureT = decltype(std::packaged_task<RawT>().get_future());
};
}
