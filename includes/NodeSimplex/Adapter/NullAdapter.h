#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Adapter {

// I am not a fan of this Node, but as for now it is required in order to
// create a block hole for signals. Think /dev/null for signals.
template <size_t __arityIn, size_t __arityOut>
class NullAdapter : public Graph::AudioNode<__arityIn, __arityOut> {

  Graph::AudioFrame<__arityOut> tick(Graph::AudioFrame<__arityIn> input) {
    return Graph::AudioFrame<__arityOut>{};
  }
};

} // namespace Clover::NodeSimplex::Adapter