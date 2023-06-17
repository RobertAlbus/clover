#pragma once

#include <algorithm>
#include <cmath>

#include "Graph.h"

namespace Clover::NodeSimplex::Envelope {

struct BasicEnvelopeSettings {
  size_t startTime;
  size_t targetTime;
  size_t duration;
  float startValue;
  float targetValue;
};

class BasicEnvelope : public StatefulProcessor<0, 1, BasicEnvelopeSettings> {
public:
  BasicEnvelope(BasicEnvelopeSettings initialSettings);

  BasicEnvelope(float currentValue, float targetValue, size_t durationTime);

  void set(float currentValue, float targetValue, size_t durationTime);

protected:
  void setState(BasicEnvelopeSettings &s, float currentValue, float targetValue,
                size_t durationTime);

  AudioFrame<1> tick(AudioFrame<0> inputFrame);
};

} // namespace Clover::NodeSimplex::Envelope