#pragma once

#include "Graph/AudioFrame.h"
#include "Graph/AudioNode.h"
#include "Graph/NullFrame.h"

namespace Clover::NodeSimplex::Adapter {

// Think /dev/null for signals.
template <size_t __arityOut>
class NullInAdapter : public Graph::AudioOutNode<__arityOut> {

  Graph::AudioFrame<__arityOut> tick(Graph::NullFrame input) {
    return Graph::AudioFrame<__arityOut>{};
  }
};

template <size_t __arityIn>
class NullOutAdapter : public Graph::AudioInNode<__arityIn> {

  Graph::NullFrame tick(Graph::AudioFrame<__arityIn> input) {
    return Graph::NullFrame{};
  }
};

} // namespace Clover::NodeSimplex::Adapter
