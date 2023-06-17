#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Basic {

template <size_t __arity> class Gain : public AudioNode<__arity, __arity> {

  AudioFrame<__arity> tick(AudioFrame<__arity> input) { return input; };
};

} // namespace Clover::NodeSimplex::Basic
