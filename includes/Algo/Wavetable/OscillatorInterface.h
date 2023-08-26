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

#include <memory>

#include "Util/FloatingPointConcept.h"

namespace Clover::Wavetable {

template <FloatingPoint T> using Wavetable = std::vector<T>;

template <FloatingPoint T> class OscillatorIndexCalculculatorInterface {
public:
  virtual void phase(T phasePercent) = 0;
  virtual T phase() = 0;
  virtual void phaseOffset(T offsetPercent) = 0;
  virtual T phaseOffset() = 0;

  virtual void freq(T freqHz) = 0;
  virtual T freq() = 0;
  virtual void period(T periodSamples) = 0;
  virtual T period() = 0;
  virtual int size() = 0;
  virtual void size(int sizeSamples) = 0;
  virtual T sampleRate() = 0;
};

template <FloatingPoint T> class WavetableOscInterface {
public:
  virtual void phase(T phasePercent) = 0;
  virtual T phase() = 0;
  virtual void phaseOffset(T offsetPercent) = 0;
  virtual T phaseOffset() = 0;

  virtual void freq(T freqHz) = 0;
  virtual T freq() = 0;
  virtual void period(T periodSamples) = 0;
  virtual T period() = 0;
  virtual int size() = 0;
  virtual T sampleRate() = 0;

  virtual void note(float midiNote) = 0;
  virtual T note() = 0;

  virtual void wavetable(std::shared_ptr<Wavetable<T>> wt) = 0;
  virtual std::shared_ptr<Wavetable<T>> wavetable() = 0;

  // clang-format on
  virtual void sine(int size = 512) = 0;
  virtual void square(int size = 512) = 0;
  virtual void saw(int size = 512) = 0;
  virtual void tri(int size = 5) = 0;
  virtual void noiseWhite(int size = 1024) = 0;
  // clang-format off
};

} // namespace Clover::Wavetable
