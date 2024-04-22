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

#include "Clover/Nodes/Envelope/ADSR.h"

namespace Clover::Nodes::Envelope {

ADSR::ADSR() : ADSR(1, 1, 1.f, 1) {}

ADSR::ADSR(int a, int d, float s, int r) : AudioNode(), envelope(a, d, s, r) {}

void ADSR::set(int a, int d, float s, int r) {
  attack(a);
  decay(d);
  sustain(s);
  release(r);
}

void ADSR::set(float a, float d, float s, float r) {
  int a_rounded = static_cast<int>(std::round(a));
  int d_rounded = static_cast<int>(std::round(a + d) - a_rounded);
  int r_rounded = static_cast<int>(std::round(r));

  attack(a_rounded);
  decay(d_rounded);
  sustain(s);
  release(r_rounded);
}

int ADSR::attack() { return envelope.attack(); }
void ADSR::attack(int a) { envelope.attack(a); }

int ADSR::decay() { return envelope.decay(); }
void ADSR::decay(int d) { envelope.decay(d); }

float ADSR::sustain() { return envelope.sustain(); }
void ADSR::sustain(float s) { envelope.sustain(s); }

int ADSR::release() { return envelope.release(); }
void ADSR::release(int r) { envelope.release(r); }

void ADSR::triggerOn() { envelope.keyOn(); }
void ADSR::triggerOff() { envelope.keyOff(); }

Graph::AudioFrame<1> ADSR::tick(Graph::AudioFrame<0> input) {
  return Graph::AudioFrame<1>{envelope.process()};
}

} // namespace Clover::Nodes::Envelope
