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

#include "Algo/Envelope/ADSR.h"
#include "Graph/AudioNode.h"
#include "Nodes/StepSequencer/lib.h"

namespace Clover::Nodes::Envelope {

class ADSR : public Triggerable, public Graph::AudioNode<0, 1> {
public:
  ADSR();
  ADSR(int a, int d, float s, int r);

  void set(float a, float d, float s, float r);
  void set(int a, int d, float s, int r);

  void attack(int a);
  void decay(int d);
  void sustain(float s);
  void release(int r);

  int attack();
  int decay();
  float sustain();
  int release();

  void triggerOn() override;
  void triggerOff() override;

protected:
  Clover::Envelope::ADSR<float> envelope;

  Graph::AudioFrame<1> tick(Graph::AudioFrame<0> input);
};

} // namespace Clover::Nodes::Envelope
