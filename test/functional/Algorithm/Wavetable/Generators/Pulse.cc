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

#include "Algo/Wavetable/Generators.h"

TEST(Algorithm_Wavetable_Generator_Pulse, ShouldBeCorrectSize) {
  EXPECT_EQ(Clover::Wavetable::Pulse<float>(8, 0.25f).size(), 8);
  EXPECT_EQ(Clover::Wavetable::Pulse<float>(1024, 0.5f).size(), 1024);
}

TEST(Algorithm_Wavetable_Generator_Pulse, ShouldHaveCorrectProportions) {
  int tableSize = 4096;
  float pulseWidth = 0.25f;
  std::vector<float> table =
      Clover::Wavetable::Pulse<float>(tableSize, pulseWidth);

  int positiveRegion = tableSize * pulseWidth;
  for (int i = 0; i <= positiveRegion; i++) {
    EXPECT_FLOAT_EQ(table[i], 1.f) << i;
  }
  for (int i = positiveRegion + 1; i < tableSize; i++) {
    EXPECT_FLOAT_EQ(table[i], -1.f) << i;
  }
}

TEST(Algorithm_Wavetable_Generator_Pulse, ShouldBeWithinSignalBoundaries) {
  int tableSize = 4096;
  float pulseWidth = 0.25f;
  std::vector<float> table =
      Clover::Wavetable::Pulse<float>(tableSize, pulseWidth);

  for (int i = 0; i < tableSize; i++) {
    EXPECT_LE(table[i], 1.f);
    EXPECT_GE(table[i], -1.f);
  }
}

TEST(Algorithm_Wavetable_Generator_Pulse, ShouldBeNormalized) {
  int tableSize = 4096;
  float pulseWidth = 0.25f;
  std::vector<float> table =
      Clover::Wavetable::Pulse<float>(tableSize, pulseWidth);

  float absMaxima = 0.f;
  for (int i = 0, end = table.size(); i < end; i++) {
    if (fabs(table[i]) > absMaxima)
      absMaxima = table[i];
  }

  EXPECT_FLOAT_EQ(absMaxima, 1.f);
}
