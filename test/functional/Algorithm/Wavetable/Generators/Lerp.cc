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

TEST(Algorithm_Wavetable_Generator_Lerp, ShouldBeCorrectSize) {
  std::vector<float> rawTable{-1, 0, 0, 1};
  std::vector<float> lerpTable =
      Clover::Wavetable::LerpTable<float>(8, rawTable);

  EXPECT_EQ(Clover::Wavetable::LerpTable<float>(8, rawTable).size(), 8);
  EXPECT_EQ(Clover::Wavetable::LerpTable<float>(1024, rawTable).size(), 1024);
}

TEST(Algorithm_Wavetable_Generator_Lerp, ShouldLerp) {
  std::vector<float> rawTable{-1, 0, 0, 1};
  int newSize = 7;
  std::vector<float> lerpTable =
      Clover::Wavetable::LerpTable<float>(newSize, rawTable);

  EXPECT_EQ(lerpTable.size(), newSize);

  EXPECT_FLOAT_EQ(lerpTable[0], -1.f);
  EXPECT_FLOAT_EQ(lerpTable[1], -0.5f);
  EXPECT_FLOAT_EQ(lerpTable[2], 0.f);
  EXPECT_FLOAT_EQ(lerpTable[3], 0.f);
  EXPECT_FLOAT_EQ(lerpTable[4], 0.f);
  EXPECT_FLOAT_EQ(lerpTable[5], 0.5f);
  EXPECT_FLOAT_EQ(lerpTable[6], 1.f);
}
