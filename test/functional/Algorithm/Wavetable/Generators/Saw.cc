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

TEST(Algorithm_Wavetable_Generator_Saw, ShouldBeCorrectSize) {
  EXPECT_EQ(Clover::Wavetable::Saw<float>(8).size(), 8);
  EXPECT_EQ(Clover::Wavetable::Saw<float>(1024).size(), 1024);
}

TEST(Algorithm_Wavetable_Generator_Saw, ShouldMoveHighToLow) {
  int tableSize = 4096;
  std::vector<float> table = Clover::Wavetable::Saw<float>(tableSize);

  EXPECT_FLOAT_EQ(table[0], 1.f);
  EXPECT_FLOAT_EQ(table[tableSize - 1], -1.f);

  for (int i = 1; i < tableSize; i++) {
    EXPECT_LT(table[i], table[i - 1]) << i;
  }
}

TEST(Algorithm_Wavetable_Generator_Saw, ShouldBeWithinSignalBoundaries) {
  int tableSize = 4096;
  std::vector<float> table = Clover::Wavetable::Saw<float>(tableSize);

  for (int i = 0; i < tableSize; i++) {
    EXPECT_LE(table[i], 1.f);
    EXPECT_GE(table[i], -1.f);
  }
}

TEST(Algorithm_Wavetable_Generator_Saw, ShouldBeNormalized) {
  int size = 127;
  std::vector<float> table = Clover::Wavetable::Saw<float>(size);

  float absMaxima = 0.f;
  for (int i = 0, end = table.size(); i < end; i++) {
    if (fabs(table[i]) > absMaxima)
      absMaxima = table[i];
  }

  EXPECT_FLOAT_EQ(absMaxima, 1.f);
}
