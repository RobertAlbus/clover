#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Adapter {

template <size_t __arityIn, size_t __arityOut>
class NullAdapter : public AudioNode<__arityIn, __arityOut> {

  AudioFrame<__arityOut> tick(AudioFrame<__arityIn> input) { return AudioFrame<__arityOut>{}; }
};

} // namespace Clover::NodeSimplex::Adapter