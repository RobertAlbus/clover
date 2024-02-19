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

#include <cmath> // lerp

#include "Clover/Graph.h"
#include "Clover/Nodes/Stereo/Pan.h"

namespace Clover::Nodes::Stereo {

Pan1::Pan1(float p) : Graph::AudioNode<1, 2>(), pan_(p) {}

Graph::AudioFrame<2> Pan1::tick(Graph::AudioFrame<1> input) {

  return Graph::AudioFrame<2>{pan_.process(input[0])};
}

void Pan1::pan(float p) { pan_.pan(p); }
float Pan1::pan() { return pan_.pan(); }

Pan2::Pan2(float p) : Graph::AudioNode<2, 2>(), pan_(p) {}

Graph::AudioFrame<2> Pan2::tick(Graph::AudioFrame<2> input) {
  return Graph::AudioFrame<2>{pan_.process(input.data)};
}

void Pan2::pan(float p) { pan_.pan(p); }
float Pan2::pan() { return pan_.pan(); }

} // namespace Clover::Nodes::Stereo
