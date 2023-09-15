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

#include <cmath>

#include "Nodes/Stereo/MidSideBalance.h"

#include "Graph/AudioFrame.h"
#include "Graph/AudioNode.h"

namespace Clover::Nodes::Stereo {

MidSideBalance::MidSideBalance(float s) : AudioNode<2, 2>(), msBalance(s) {}

void MidSideBalance::spread(float s) { msBalance.spread(s); }

float MidSideBalance::spread() { return msBalance.spread(); }

Graph::AudioFrame<2> MidSideBalance::tick(Graph::AudioFrame<2> input) {
  return msBalance.process(input.data);
}

} // namespace Clover::Nodes::Stereo
