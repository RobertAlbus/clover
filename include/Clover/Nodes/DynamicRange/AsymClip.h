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

#include "Clover/Algorithm/DynamicRange/AsymClip.h"
#include "Clover/Graph/AudioNode.h"

namespace Clover::Nodes::DynamicRange {

template <size_t __arity>
class AsymClip : public Graph::AudioNode<__arity, __arity> {
public:
  AsymClip() : AsymClip(-1.f, 1.f) {}

  AsymClip(float minimum, float maximum)
      : Graph::AudioNode<__arity, __arity>(), clip(minimum, maximum) {}

  float min() { return clip.min(); }
  float max() { return clip.max(); }

  void min(float minimum) { clip.min(minimum); }
  void max(float maximum) { clip.max(maximum); }

private:
  Clover::DynamicRange::AsymClip<Sample, __arity> clip;

  Graph::AudioFrame<__arity> tick(Graph::AudioFrame<__arity> input) {
    return Graph::AudioFrame<__arity>{clip.process(input.data)};
  }
};

} // namespace Clover::Nodes::DynamicRange
