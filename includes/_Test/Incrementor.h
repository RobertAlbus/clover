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

#include "Graph/AudioFrame.h"
#include "Graph/AudioNode.h"
#include "Graph/NullFrame.h"

namespace Clover::_Test {

template <size_t __arityOut>
class Incrementor : public Graph::AudioOutNode<__arityOut> {
public:
  Incrementor(int startValue = 0)
      : Graph::AudioOutNode<__arityOut>(), basis(startValue) {}

private:
  float basis;
  Graph::AudioFrame<__arityOut> tick(Graph::NullFrame input) {
    Graph::AudioFrame<__arityOut> f{};

    float time = (float)this->_currentClockTime;
    for (size_t i = 0; i < __arityOut; i++) {
      f[i] = time + basis;
    }

    return f;
  }
};

} // namespace Clover::_Test
