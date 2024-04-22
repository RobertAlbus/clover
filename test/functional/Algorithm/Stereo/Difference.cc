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

#include "Clover/Algorithm/Stereo/Difference.h"

TEST(Algorithm_Stereo_Difference, ShouldPerformDifferenceCalculation) {
  Clover::Stereo::Difference<float> difference;

  std::array<float, 2> frame{1.f, -1.f};
  std::array<float, 2> result = difference.process(frame);

  EXPECT_FLOAT_EQ(result[0], 1.f);
  EXPECT_FLOAT_EQ(result[1], -1.f);

  frame[0] = 1.f;
  frame[1] = 0.5f;
  result = difference.process(frame);

  EXPECT_FLOAT_EQ(result[0], 0.25f);
  EXPECT_FLOAT_EQ(result[1], -0.25f);

  frame[0] = -1.f;
  frame[1] = -1.f;
  result = difference.process(frame);

  EXPECT_FLOAT_EQ(result[0], 0.f);
  EXPECT_FLOAT_EQ(result[1], 0.f);
}
