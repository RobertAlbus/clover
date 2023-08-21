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

namespace Clover::NodeSimplex::Adapter {

// Think /dev/null for signals.
template <size_t __arityOut>
class NullInAdapter : public Graph::AudioOutNode<__arityOut> {

  Graph::AudioFrame<__arityOut> tick(Graph::NullFrame input) {
    return Graph::AudioFrame<__arityOut>{};
  }
};

template <size_t __arityIn>
class NullOutAdapter : public Graph::AudioInNode<__arityIn> {

  Graph::NullFrame tick(Graph::AudioFrame<__arityIn> input) {
    return Graph::NullFrame{};
  }
};

} // namespace Clover::NodeSimplex::Adapter
