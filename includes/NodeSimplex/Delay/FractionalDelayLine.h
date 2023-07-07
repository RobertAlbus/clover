#pragma once

#include <algorithm>
#include <math.h>
#include <vector>

#include "Algo/Delay/Fractional.h"
#include "Graph.h"

namespace Clover::NodeSimplex::Delay {

template <size_t __arity>
class FractionalDelayLine : public Graph::AudioNode<__arity, __arity> {
public:
  FractionalDelayLine(int bufferCapacity, float delaySamples)
      : Graph::AudioNode<__arity, __arity>(),
        delayAlgorithm(bufferCapacity, delaySamples) {}

  void delay(float time) { delayAlgorithm.delay(time); }
  float delay() { return delayAlgorithm.delay(); }

protected:
  Clover::Delay::FractionalDelay<Sample, __arity> delayAlgorithm;
  Graph::AudioFrame<__arity> tick(Graph::AudioFrame<__arity> input) {
    return Graph::AudioFrame<__arity>{delayAlgorithm.process(input.data)};
  }
};

} // namespace Clover::NodeSimplex::Delay