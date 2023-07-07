#pragma once

#include "Graph/AudioNode.h"

namespace Clover::IO {

template <size_t __arity>
class RootNode : public Graph::AudioNode<__arity, __arity> {
public:
  /// RootNode exposes metaTick publicly as the starting point of the signal
  /// graph
  void metaTick(int currentTime) {
    Graph::AudioNode<__arity, __arity>::metaTick(currentTime);
  }

protected:
  Graph::AudioFrame<__arity> tick(Graph::AudioFrame<__arity> input) {
    return input;
  }
};

} // namespace Clover::IO