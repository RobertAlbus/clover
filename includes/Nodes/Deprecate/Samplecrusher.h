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

#include "Graph.h"

namespace Clover::Nodes::Reduce {

class Samplecrusher : public Graph::AudioNode<1, 1> {
public:
  Samplecrusher(float range = 1)
      : AudioNode(), range(range), amount(0), heldSample(0), samplesToHold(0) {}

  float range;
  float amount;
  Sample heldSample;
  float samplesToHold;

protected:
  Graph::AudioFrame<1> tick(Graph::AudioFrame<1> inputFrame) {
    if (samplesToHold < 1.) {
      samplesToHold += 1 + std::max((range * amount), 0.f);
      heldSample = inputFrame[0];
    }
    samplesToHold--;
    return Graph::AudioFrame<1>{heldSample};
  }
};

} // namespace Clover::Nodes::Reduce
