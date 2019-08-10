#pragma once

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
}
