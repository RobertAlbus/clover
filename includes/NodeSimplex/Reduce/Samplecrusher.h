#pragma once

#include <algorithm>

#include "Graph.h"

namespace Clover::NodeSimplex::Reduce {

class Samplecrusher : public AudioNode<1, 1> {
public:
  Samplecrusher(float range = 1)
      : AudioNode(), range(range), amount(0), heldSample(0), samplesToHold(0) {}

  float range;
  float amount;
  Sample heldSample;
  float samplesToHold;

protected:
  AudioFrame<1> tick(AudioFrame<1> inputFrame) {
    if (samplesToHold < 1.) {
      samplesToHold += 1 + std::max((range * amount), 0.f);
      heldSample = inputFrame[0];
    }
    samplesToHold--;
    return AudioFrame<1>{heldSample};
  }
};

} // namespace Clover::NodeSimplex::Reduce