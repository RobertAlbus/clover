#pragma once

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
