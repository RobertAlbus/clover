#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Basic {

template <size_t __arity>
class Gain : public StatelessProcessorV2<__arity, __arity> {
  Frame<__arity> processFn(Frame<__arity> input) { return input; };
};

} // namespace Clover::NodeSimplex::Basic
