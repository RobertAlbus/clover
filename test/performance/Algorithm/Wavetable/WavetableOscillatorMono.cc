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

#include "Algo/Wavetable/OscillatorInterface.h"
#include "Algo/Wavetable/WavetableOscillatorMono.h"

TEST(Algorithm_Wavetable_Oscillator_Mono, ShouldBeFast) {
  float sampleRate = 48000.f;
  int minutesOfAudio = 10;
  int testTimeInSamples = static_cast<int>(sampleRate) * 60 * minutesOfAudio;
  Clover::Wavetable::WavetableOscillatorMono<float> osc(sampleRate);

  osc.freq(100);

  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < testTimeInSamples; i++) {
    osc.process();
  }
  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::minutes audioTime(minutesOfAudio);
  std::chrono::milliseconds audioTimeMilliseconds(audioTime);
  std::chrono::milliseconds computedTime =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  printf("\nAudio Time: %i milliseconds", audioTimeMilliseconds.count());
  printf("\nComputed Time: %i milliseconds", computedTime.count());

  bool isRealTime = computedTime < audioTime;
  ASSERT_TRUE(isRealTime);
}
