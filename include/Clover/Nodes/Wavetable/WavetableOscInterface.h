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

#include <cmath>
#include <functional>
#include <memory>
#include <tgmath.h>
#include <vector>

#include "Clover/Constants.h"

namespace Clover::Nodes::Wavetable {

typedef std::vector<Sample> Wavetable;

class WavetableOscInterface {
public:
  virtual void phase(float phase) = 0;
  virtual float phase() = 0;
  virtual void phaseOffset(float offset) = 0;
  virtual float phaseOffset() = 0;

  virtual void freq(float freq) = 0;
  virtual float freq() = 0;
  virtual void note(float midiNote) = 0;
  virtual float note() = 0;
  virtual void wavelength(float wavelengthSamples) = 0;
  virtual float wavelength() = 0;

  virtual void wavetable(std::shared_ptr<Wavetable> wt) = 0;
  virtual std::shared_ptr<Wavetable> wavetable() = 0;

  virtual void sine(int size = 512) = 0;
  virtual void square(int size = 512) = 0;
  virtual void saw(int size = 512) = 0;
  virtual void tri(int size = 5) = 0;
  virtual void noise(int size = 1024) = 0;
};

} // namespace Clover::Nodes::Wavetable
