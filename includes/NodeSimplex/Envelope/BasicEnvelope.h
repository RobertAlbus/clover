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
  BasicEnvelope(BasicEnvelopeSettings initialSettings)
      : StatefulProcessor(initialSettings) {
    set(initialSettings.startValue, initialSettings.targetValue,
        initialSettings.duration);
  }

  BasicEnvelope(float currentValue, float targetValue, size_t durationTime)
      : StatefulProcessor() {
    BasicEnvelopeSettings &s = this->settings.initial;
    setState(s, currentValue, targetValue, durationTime);
    settings.reset();
  }

  void set(float currentValue, float targetValue, size_t durationTime) {
    BasicEnvelopeSettings &s = settings.current;
    setState(s, currentValue, targetValue, durationTime);
  }

protected:
  void setState(BasicEnvelopeSettings &s, float currentValue, float targetValue,
                size_t durationTime) {
    s.targetValue = targetValue;
    s.startValue = currentValue;
    s.duration = std::max(durationTime, 0UL);
    s.startTime = _currentClockTime + 1, 0;
    s.targetTime = s.startTime + s.duration - 1;
  }

  Frame<1> tick(Frame<0> inputFrame) {
    BasicEnvelopeSettings &s = settings.current;
    int elapsedTime = _currentClockTime - (int)s.startTime;
    float lerpAmount = (float)elapsedTime / (float)s.duration;
    if (elapsedTime < (float)s.duration) {
      Frame<1> f{std::lerp(s.startValue, s.targetValue, lerpAmount)};
      return f;
    }
    return Frame<1>{s.targetValue};
  }
};

} // namespace Clover::NodeSimplex::Envelope