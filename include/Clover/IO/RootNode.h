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

#include "Clover/Graph/AudioNode.h"

namespace Clover::IO {

template <size_t __arity>
class RootNode : public Graph::AudioNode<__arity, __arity> {
public:
  /// RootNode exposes metaTick publicly as the starting point of the signal
  /// graph
  void metaTick(int currentTime) {
    Graph::AudioNode<__arity, __arity>::metaTick(currentTime);
  }

protected:
  Graph::AudioFrame<__arity> tick(Graph::AudioFrame<__arity> input) {
    return input;
  }
};

} // namespace Clover::IO
