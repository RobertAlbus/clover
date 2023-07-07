#include <algorithm>
#include <cmath>

#include "Graph/AudioFrame.h"
#include "Graph/AudioNode.h"
#include "NodeSimplex/Envelope/BasicEnvelope.h"

namespace Clover::NodeSimplex::Envelope {

BasicEnvelope::BasicEnvelope() : BasicEnvelope(0.f, 0.f, 0) {}

BasicEnvelope::BasicEnvelope(float currentValue, float targetValue, uint durationTime) {
  set(currentValue, targetValue, durationTime);
}

void BasicEnvelope::set(float targetValue, uint durationTime) {
  envelope.set(targetValue, durationTime);
}

void BasicEnvelope::set(float currentValue, float targetValue, uint durationTime) {
  envelope.set(currentValue, targetValue, durationTime);
}

Graph::AudioFrame<1> BasicEnvelope::tick(Graph::AudioFrame<0> inputFrame) {
  return Graph::AudioFrame<1>{envelope.process()};
}

} // namespace Clover::NodeSimplex::Envelope