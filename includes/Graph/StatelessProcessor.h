#pragma once

#include <functional>

#include "Graph.h"

namespace Clover::Graph {

/// A Node with a transform function that does not maintain any inter-sample
/// state.

template <size_t __arityIn, size_t __arityOut>
class StatelessProcessorV2 : public AudioNode<__arityIn, __arityOut> {
public:
  StatelessProcessorV2() : AudioNode<__arityIn, __arityOut>() {}

protected:
  AudioFrame<__arityOut> tick(AudioFrame<__arityIn> input) { return processFn(input); };
  virtual AudioFrame<__arityOut> processFn(AudioFrame<__arityIn>) = 0;
};

} // namespace Clover::Graph
