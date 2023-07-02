#include <algorithm>
#include <cmath>

#include "Graph.h"
#include "NodeSimplex/Envelope/BasicEnvelope.h"

namespace Clover::NodeSimplex::Envelope {


BasicEnvelope::BasicEnvelope(float currentValue, float targetValue, uint durationTime) {
  set(currentValue, targetValue, durationTime);
}

void BasicEnvelope::set(float currentValue, float targetValue, uint durationTime) {
  envelope.set(currentValue, targetValue, durationTime);
}

AudioFrame<1> BasicEnvelope::tick(AudioFrame<0> inputFrame) {
    printf("\n %i", _currentClockTime);

  return AudioFrame<1>{envelope.process()};
}

} // namespace Clover::NodeSimplex::Envelope