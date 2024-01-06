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
#include <math.h>
#include <vector>

#include "Algorithm/Delay/Fractional.h"
#include "Graph.h"

namespace Clover::Nodes::Delay {

template <size_t __arity>
class FractionalDelayLine : public Graph::AudioNode<__arity, __arity> {
public:
  FractionalDelayLine(int bufferCapacity, float delaySamples)
      : Graph::AudioNode<__arity, __arity>(),
        delayAlgorithm(bufferCapacity, delaySamples) {}

  void delay(float time) { delayAlgorithm.delay(time); }
  float delay() { return delayAlgorithm.delay(); }

protected:
  Clover::Delay::FractionalDelay<Sample, __arity> delayAlgorithm;
  Graph::AudioFrame<__arity> tick(Graph::AudioFrame<__arity> input) {
    return Graph::AudioFrame<__arity>{delayAlgorithm.process(input.data)};
  }
};

} // namespace Clover::Nodes::Delay
