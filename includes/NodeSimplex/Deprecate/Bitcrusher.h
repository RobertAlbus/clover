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


#include <limits.h>
#include <math.h>

#include "Graph.h"
#include "Util/Calc.h"

namespace Clover::NodeSimplex::Reduce {

class BitCrusher : public Graph::AudioNode<1, 1> {
public:
  BitCrusher(float bits = std::numeric_limits<float>::max())
      : AudioNode(), bits(bits) {}

  // generally undetectable at about 600-800
  // I should probably convert this to an "amount" control
  // I can fade out the crush signal in the highest 10%
  // I can constrain the bits value to sensible values

  // I might need to do something more extreme for the high-effect
  // position to actually sound decent. the crush is anemic.
  float bits;

protected:
  Graph::AudioFrame<1> tick(Graph::AudioFrame<1> inputFrame) {
    float sign = Util::Calc::sign(inputFrame[0]);
    float abs = fabs(inputFrame[0]);
    abs = floor(abs * bits) / bits;
    return Graph::AudioFrame<1>{abs * sign};
  }
};

} // namespace Clover::NodeSimplex::Reduce
