/*
 * /////////
 * // Clover
 *
 * Audio processing algorithms and DAG with feedback loops that do not break
 * acyclicity.
 *
 * Copyright (C) 2023 Rob W. Albus
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version. This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <algorithm>
#include <cmath>

#include "Graph/AudioFrame.h"
#include "Graph/AudioNode.h"
#include "Nodes/Envelope/BasicEnvelope.h"

namespace Clover::Nodes::Envelope {

BasicEnvelope::BasicEnvelope() : BasicEnvelope(0.f, 0.f, 0) {}

BasicEnvelope::BasicEnvelope(float currentValue, float targetValue,
                             uint durationTime) {
  set(currentValue, targetValue, durationTime);
}

void BasicEnvelope::set(float targetValue, uint durationTime) {
  envelope.set(targetValue, durationTime);
}

void BasicEnvelope::set(float currentValue, float targetValue,
                        uint durationTime) {
  envelope.set(currentValue, targetValue, durationTime);
}

Graph::AudioFrame<1> BasicEnvelope::tick(Graph::AudioFrame<0> inputFrame) {
  return Graph::AudioFrame<1>{envelope.process()};
}

} // namespace Clover::Nodes::Envelope
