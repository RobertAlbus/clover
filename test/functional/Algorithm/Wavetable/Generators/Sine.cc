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

#include "Algorithm/Wavetable/Generators.h"

TEST(Algorithm_Wavetable_Generator_Sine, ShouldBeCorrectSize) {
  EXPECT_EQ(Clover::Wavetable::Sine<float>(8).size(), 8);
  EXPECT_EQ(Clover::Wavetable::Sine<float>(1024).size(), 1024);
}

TEST(Algorithm_Wavetable_Generator_Sine, ShouldHaveCorrectRMS) {
  int size = 4096;
  std::vector<float> sineTable = Clover::Wavetable::Sine<float>(size);

  float squareAccumulator = 0;
  for (int i = 0, end = sineTable.size(); i < end; i++) {
    float item = sineTable[i];
    squareAccumulator += item * item;
  }

  float rms = sqrt(squareAccumulator / static_cast<float>(sineTable.size()));
  float expectedRms = 1.f / sqrtf(2.f);

  EXPECT_FLOAT_EQ(rms, expectedRms);
}

TEST(Algorithm_Wavetable_Generator_Sine, ShouldBeWithinSignalBoundaries) {
  int size = 4096;
  std::vector<float> sineTable = Clover::Wavetable::Sine<float>(size);

  for (int i = 0, end = sineTable.size(); i < end; i++) {
    EXPECT_LE(sineTable[i], 1.f);
    EXPECT_GE(sineTable[i], -1.f);
  }
}

TEST(Algorithm_Wavetable_Generator_Sine, ShouldBeNormalized) {
  // a table size of 127 results in minima/maxima +/- 0.99992353
  int size = 127;
  std::vector<float> table = Clover::Wavetable::Sine<float>(size);

  float absMaxima = 0.f;
  for (int i = 0, end = table.size(); i < end; i++) {
    if (fabs(table[i]) > absMaxima)
      absMaxima = table[i];
  }

  EXPECT_FLOAT_EQ(absMaxima, 1.f);
}
