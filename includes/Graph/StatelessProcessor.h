#pragma once

#include <functional>

#include "Graph.h"

namespace Clover::Graph {

/// A Node with a transform function that does not maintain any inter-sample
/// state.

template <size_t __arityIn, size_t __arityOut>
class StatelessProcessorV2 : public Node<__arityIn, __arityOut> {
public:
  StatelessProcessorV2() : Node<__arityIn, __arityOut>() {}

protected:
  Frame<__arityOut> tick(Frame<__arityIn> input) { return processFn(input); };
  virtual Frame<__arityOut> processFn(Frame<__arityIn>) = 0;
};

} // namespace Clover::Graph
