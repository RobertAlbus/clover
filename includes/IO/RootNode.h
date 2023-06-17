#pragma once

#include "Graph.h"

using namespace Clover::Graph;

namespace Clover::IO {

template <size_t __arity> class RootNode : public AudioNode<__arity, __arity> {
public:
  /// RootNode exposes metaTick publicly as the starting point of the signal
  /// graph
  void metaTick(int currentTime) {
    AudioNode<__arity, __arity>::metaTick(currentTime);
  }

protected:
  AudioFrame<__arity> tick(AudioFrame<__arity> input) { return input; }
};

} // namespace Clover::IO