#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Adapter {

// I am not a fan of this Node, but as for now it is required in order to
// create a block hole for signals. Think /dev/null for signals.
template <size_t __arityIn, size_t __arityOut>
class NullAdapter : public AudioNode<__arityIn, __arityOut> {

  AudioFrame<__arityOut> tick(AudioFrame<__arityIn> input) {
    return AudioFrame<__arityOut>{};
  }
};

} // namespace Clover::NodeSimplex::Adapter