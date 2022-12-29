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
    set(currentValue, targetValue, durationTime);
  }

  void set(float targetValue) {
    target(targetValue);
    updateTargetTime();
  }
  void reset(float targetValue) { updateTargetTime(); }
  void set(float targetValue, size_t durationTime) {
    target(targetValue);
    dur(durationTime);
    updateTargetTime();
  }
  void set(float currentValue, float targetValue, size_t durationTime) {
    target(targetValue);
    dur(durationTime);
    current(currentValue);
    updateTargetTime();
  }

protected:
  void target(float t) { settings.current.targetValue = t; }

  void current(float currentValue) {
    settings.current.startValue = currentValue;
  }

  void dur(size_t d) { settings.current.duration = std::max(d, 0UL); }

  void updateTargetTime() {
    BasicEnvelopeSettings &s = settings.current;
    s.startTime = _currentClockTime;
    s.targetTime = s.startTime + s.duration;
  }

  Frame<1> tick(Frame<0> inputFrame) {
    BasicEnvelopeSettings &s = settings.current;
    float elapsedTime = (float)_currentClockTime - (float)s.startTime;

    if (_currentClockTime > s.targetTime) {
      return Frame<1>{s.targetValue};
    }

    if (elapsedTime <= 0.f) {
      return Frame<1>{s.startValue};
    }

    float lerpAmount = (elapsedTime / (float)s.duration);
    lerpAmount = isinf(lerpAmount) ? 0. : lerpAmount;

    return Frame<1>{std::lerp(s.startValue, s.targetValue, lerpAmount)};
  }
};

} // namespace Clover::NodeSimplex::Envelope