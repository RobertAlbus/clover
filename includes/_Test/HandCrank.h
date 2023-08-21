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


#include "Graph/NullFrame.h"
#include "Graph/NullNode.h"
#include "Util/SampleClock.h"

namespace Clover::_Test {

class HandCrank : public Graph::NullNode {
public:
  HandCrank() : Graph::NullNode() {}

  void turn(int numberOfTicks) {
    for (int i = 0; i < numberOfTicks; i++) {
      this->metaTick(clock.currentSample());
      clock.tick();
    }
  }

private:
  Clover::Util::SampleClock clock;

  Graph::NullFrame tick(Graph::NullFrame input) { return Graph::NullFrame{}; }
};

} // namespace Clover::_Test
