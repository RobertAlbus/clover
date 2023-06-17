#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Basic {

template <size_t __arity> class Gain : public AudioNode<__arity, __arity> {

  Frame<__arity> tick(Frame<__arity> input) { return input; };
};

} // namespace Clover::NodeSimplex::Basic
