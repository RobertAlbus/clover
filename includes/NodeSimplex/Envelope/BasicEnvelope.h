#pragma once

#include <algorithm>
#include <cmath>

#include "Algorithm.h"
#include "Graph.h"

namespace Clover::NodeSimplex::Envelope {

class BasicEnvelope : public AudioNode<0, 1> {
public:
  BasicEnvelope();
  BasicEnvelope(float currentValue, float targetValue, uint durationTime);

  void set(float targetValue, uint durationTimeSamples);
  void set(float currentValue, float targetValue, uint durationTimeSamples);

protected:
  AudioFrame<1> tick(AudioFrame<0> inputFrame);
  Clover::Envelope::Basic<Sample> envelope;
};

} // namespace Clover::NodeSimplex::Envelope