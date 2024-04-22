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

#include "Clover/Algorithm/Stereo/MidSideBalance.h"

TEST(Algorithm_Stereo_Spread, ShouldInitialize) {
  Clover::Stereo::MidSideBalance<float> defaultConstructor;
  EXPECT_FLOAT_EQ(defaultConstructor.spread(), 0.f);

  Clover::Stereo::MidSideBalance<float> explicitConstructor(-1.f);
  EXPECT_FLOAT_EQ(explicitConstructor.spread(), -1.f);
}

TEST(Algorithm_Stereo_Spread, ShouldSpreadInvertedSignal) {
  Clover::Stereo::MidSideBalance<float> image;

  std::array<float, 2> inputFrame{1.f, -1.f};
  std::array<float, 2> result{0.f, 0.f};

  image.spread(-1.f);
  result = image.process(inputFrame);

  EXPECT_FLOAT_EQ(result[0], 0.f);
  EXPECT_FLOAT_EQ(result[1], 0.f);

  image.spread(1.f);
  result = image.process(inputFrame);

  EXPECT_FLOAT_EQ(result[0], 1.f);
  EXPECT_FLOAT_EQ(result[1], -1.f);
}

TEST(Algorithm_Stereo_Spread, ShouldNotSpreadMonoSignal) {
  Clover::Stereo::MidSideBalance<float> image;

  std::array<float, 2> frame{1.f, 1.f};
  std::array<float, 2> result{0.f, 0.f};

  image.spread(-1.f);
  result = image.process(frame);

  EXPECT_FLOAT_EQ(result[0], 1.f);
  EXPECT_FLOAT_EQ(result[1], 1.f);

  image.spread(1.f);
  result = image.process(frame);
  EXPECT_FLOAT_EQ(result[0], 0.f);
  EXPECT_FLOAT_EQ(result[1], 0.f);
}

TEST(Algorithm_Stereo_Spread, ShouldSpreadStereoSignal) {
  Clover::Stereo::MidSideBalance<float> image;

  std::array<float, 2> frame{1.f, 0.9f};
  std::array<float, 2> result{0.f, 0.f};

  image.spread(-1.f);
  result = image.process(frame);

  EXPECT_FLOAT_EQ(result[0], 0.95);
  EXPECT_FLOAT_EQ(result[1], 0.95);

  image.spread(1.f);
  result = image.process(frame);

  EXPECT_FLOAT_EQ(result[0], 0.05f);
  EXPECT_FLOAT_EQ(result[1], -0.05f);
}
