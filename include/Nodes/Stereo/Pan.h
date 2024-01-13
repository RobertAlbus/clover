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

#include "Algorithm/Stereo/Pan.h"
#include "Graph/AudioFrame.h"
#include "Graph/AudioNode.h"

namespace Clover::Nodes::Stereo {

class Pan1 : public Graph::AudioNode<1, 2> {
public:
  Pan1(float p = 0.f);

  void pan(float p);
  float pan();

private:
  Graph::AudioFrame<2> tick(Graph::AudioFrame<1> input);
  Clover::Stereo::PanMono<float> pan_;
};

class Pan2 : public Graph::AudioNode<2, 2> {
public:
  Pan2(float p = 0.f);

  void pan(float p);
  float pan();

private:
  Graph::AudioFrame<2> tick(Graph::AudioFrame<2> input);
  Clover::Stereo::PanStereo<float> pan_;
};

} // namespace Clover::Nodes::Stereo
