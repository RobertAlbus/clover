#pragma once

#include <algorithm>
#include <cmath>

#include "Algo/Envelope/Basic.h"
#include "Graph/AudioFrame.h"
#include "Graph/AudioNode.h"

namespace Clover::NodeSimplex::Envelope {

class BasicEnvelope : public Graph::AudioNode<0, 1> {
public:
  BasicEnvelope();
  BasicEnvelope(float currentValue, float targetValue, uint durationTime);

  void set(float targetValue, uint durationTimeSamples);
  void set(float currentValue, float targetValue, uint durationTimeSamples);

protected:
  Graph::AudioFrame<1> tick(Graph::AudioFrame<0> inputFrame);
  Clover::Envelope::Basic<Sample> envelope;
};

} // namespace Clover::NodeSimplex::Envelope