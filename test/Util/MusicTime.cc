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

#include "Clover.h"

TEST(Util_MusicTime, ShouldProvideCorrectTime) {
  float sampleRate = 48000.0;
  float minute = 60. * sampleRate;
  float bpm = 159.;

  float beatLength = minute / bpm;

  Clover::Util::SampleClock clock;
  Clover::Util::Time time(bpm, (int)sampleRate, &clock);

  int duration_two_and_a_half_beats = (int)(beatLength * 2.5);
  for (int i = 0; i < duration_two_and_a_half_beats; i++) {
    clock.tick();
  }

  EXPECT_EQ(time.beat(), beatLength);
  EXPECT_EQ((float)time.beat_i(), (int)beatLength);
  EXPECT_EQ(time.currentBeat(),
            ((float)duration_two_and_a_half_beats / time.beat()));
  EXPECT_EQ(time.currentBeat_i(), 2);
}
