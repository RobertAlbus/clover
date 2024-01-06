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

#include <gtest/gtest.h>

#include <vector>

#include "Algorithm/Wavetable/OscillatorInterface.h"
#include "Algorithm/Wavetable/WavetableOscillatorMono.h"

TEST(Algorithm_Wavetable_Oscillator_Mono, ShouldOutputSine) {
  float sampleRate = 48000.f;
  int size = 512;
  Clover::Wavetable::WavetableOscillatorMono<float> osc(sampleRate);

  osc.freq(100);

  int numSamples = sampleRate * 2;
  float accumulator = 0.f;
  for (int i = 0; i < numSamples; i++) {
    float val = osc.process();
    accumulator += (val * val);
  }
  float RMS = sqrt(accumulator / static_cast<float>(numSamples));
  float expectedRms = 1.f / sqrtf(2.f);

  ASSERT_NEAR(RMS, expectedRms, 0.000107f);
}
