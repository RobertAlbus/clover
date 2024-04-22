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

#include <array>

#include "Clover/Algorithm/Stereo/Sum.h"

TEST(Algorithm_Sum_Mono, ShouldPerformSumCalculation) {
  Clover::Stereo::SumMono<float> sum;

  // sum to null
  std::array<float, 2> frame{1.f, -1.f};
  float result = sum.process(frame);

  EXPECT_FLOAT_EQ(result, 0.f);

  // sum to mono to mono
  frame[0] = -1.f;
  frame[1] = -1.f;
  result = sum.process(frame);

  EXPECT_FLOAT_EQ(result, -1.f);

  // sum stereo to mono
  frame[0] = 1.f;
  frame[1] = 0.5f;
  result = sum.process(frame);

  EXPECT_FLOAT_EQ(result, 0.75f);
}

TEST(Algorithm_Sum_Stereo, ShouldPerformSumCalculation) {
  Clover::Stereo::SumStereo<float> sum;

  // sum to null
  std::array<float, 2> frame{1.f, -1.f};
  std::array<float, 2> result = sum.process(frame);

  EXPECT_FLOAT_EQ(result[0], 0.f);
  EXPECT_FLOAT_EQ(result[1], 0.f);

  // sum to mono to mono
  frame[0] = -1.f;
  frame[1] = -1.f;
  result = sum.process(frame);

  EXPECT_FLOAT_EQ(result[0], -1.f);
  EXPECT_FLOAT_EQ(result[1], -1.f);

  // sum stereo to mono
  frame[0] = 1.f;
  frame[1] = 0.5f;
  result = sum.process(frame);

  EXPECT_FLOAT_EQ(result[0], 0.75f);
  EXPECT_FLOAT_EQ(result[1], 0.75f);
}
