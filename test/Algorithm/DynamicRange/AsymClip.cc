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

#include "Algo/DynamicRange/AsymClip.h"

TEST(Algorithm_Dynamic_Range_Clip, ShouldInitialize) {
  Clover::DynamicRange::AsymClip<float, 1> clip;

  EXPECT_EQ(clip.min(), -1.f);
  EXPECT_EQ(clip.max(), 1.f);
}

TEST(Algorithm_Dynamic_Range_Clip, ShouldClipAsymetrically) {
  Clover::DynamicRange::AsymClip<float, 1> clip;
  clip.min(-0.9f);
  clip.max(0.95f);

  std::array<float, 1> within{0.5f};
  std::array<float, 1> at{0.95f};
  std::array<float, 1> exceed{1.5f};

  std::array<float, 1> negative_within{-0.5f};
  std::array<float, 1> negative_at{-0.9f};
  std::array<float, 1> negative_exceed{-1.5f};

  EXPECT_EQ(clip.process(within), within);
  EXPECT_EQ(clip.process(at), at);
  EXPECT_EQ(clip.process(exceed), at);

  EXPECT_EQ(clip.process(negative_exceed), negative_at);
  EXPECT_EQ(clip.process(negative_at), negative_at);
  EXPECT_EQ(clip.process(negative_within), negative_within);
}
