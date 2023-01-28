#include <algorithm>
#include <cmath>

#include "Graph.h"
#include "NodeSimplex/Envelope/BasicEnvelope.h"

namespace Clover::NodeSimplex::Envelope {

BasicEnvelope::BasicEnvelope(BasicEnvelopeSettings initialSettings)
    : StatefulProcessor(initialSettings) {
  set(initialSettings.startValue, initialSettings.targetValue,
      initialSettings.duration);
}

BasicEnvelope::BasicEnvelope(float currentValue, float targetValue, size_t durationTime)
    : StatefulProcessor() {
  BasicEnvelopeSettings &s = this->settings.initial;
  setState(s, currentValue, targetValue, durationTime);
  settings.reset();
}

void BasicEnvelope::set(float currentValue, float targetValue, size_t durationTime) {
  BasicEnvelopeSettings &s = settings.current;
  setState(s, currentValue, targetValue, durationTime);
}

void BasicEnvelope::setState(BasicEnvelopeSettings &s, float currentValue, float targetValue,
              size_t durationTime) {
  s.targetValue = targetValue;
  s.startValue = currentValue;
  s.duration = std::max(durationTime, 0UL);
  s.startTime = _currentClockTime + 1, 0;
  s.targetTime = s.startTime + s.duration - 1;
}

Frame<1> BasicEnvelope::tick(Frame<0> inputFrame) {
  BasicEnvelopeSettings &s = settings.current;
  int elapsedTime = _currentClockTime - (int)s.startTime;
  float lerpAmount = (float)elapsedTime / (float)s.duration;
  if (elapsedTime < (float)s.duration) {
    Frame<1> f{std::lerp(s.startValue, s.targetValue, lerpAmount)};
    return f;
  }
  return Frame<1>{s.targetValue};
}

} // namespace Clover::NodeSimplex::Envelope