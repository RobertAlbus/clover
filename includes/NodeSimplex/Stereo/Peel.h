#pragma once

// this file is kind of unneeded given that I have MapAdapter

#include "NodeSimplex.h"

namespace Clover::NodeSimplex::Stereo {

template <size_t __arityIn, size_t channel>
auto peelFn = [](Frame<__arityIn> input) { return Frame<1>{input[channel]}; };

template <size_t __arity>
class PeelL : public StatelessProcessor<__arity, 1, peelFn<__arity, 0>> {};

template <size_t __arity>
class PeelR : public StatelessProcessor<__arity, 1, peelFn<__arity, 1>> {};

} // namespace Clover::NodeSimplex::Stereo