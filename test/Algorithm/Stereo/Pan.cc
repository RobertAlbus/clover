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

#include "Algo/Stereo/Pan.h"

TEST(Algorithm_Pan_Base, ShouldInitializeAtZero) {
  Clover::Stereo::PanBase<float> pan;

  EXPECT_FLOAT_EQ(pan.pan(), 0.f);
}

TEST(Algorithm_Pan_Base, ShouldInitializeAtValue) {
  std::vector<float> initValues{0.f, 0.5f, 1.f, -0.5f, -1.f};
  for (int i = 0, end = initValues.size(); i < end; i++) {
    Clover::Stereo::PanBase<float> pan(initValues[i]);
    EXPECT_FLOAT_EQ(pan.pan(), initValues[i]);
  }
}

TEST(Algorithm_Pan_Base, ShouldClampInitValuesExceedingBoundaries) {
  std::vector<float> initValuesNegative{-1.1f, -1000.f};
  for (int i = 0, end = initValuesNegative.size(); i < end; i++) {
    Clover::Stereo::PanBase<float> pan(initValuesNegative[i]);
    EXPECT_FLOAT_EQ(pan.pan(), -1.f);
  }

  std::vector<float> initValuesPositive{1.1f, 1000.f};
  for (int i = 0, end = initValuesNegative.size(); i < end; i++) {
    Clover::Stereo::PanBase<float> pan(initValuesPositive[i]);
    EXPECT_FLOAT_EQ(pan.pan(), 1.f);
  }
}

TEST(Algorithm_Pan_Base, ShouldAssignPanAtValue) {
  Clover::Stereo::PanBase<float> pan;

  for (int i = 0; i < 21; i++) {
    float panValue = -1.f + (0.1f * i);
    pan.pan(panValue);
    EXPECT_FLOAT_EQ(pan.pan(), panValue);
  }
}

TEST(Algorithm_Pan_Base, ShouldClampPanAtValue) {
  Clover::Stereo::PanBase<float> pan;

  for (int i = 0; i < 10; i++) {
    float panValue = 1.f + (0.1f * (i + 1));
    pan.pan(panValue);
    EXPECT_FLOAT_EQ(pan.pan(), 1.f);

    pan.pan(-panValue);
    EXPECT_FLOAT_EQ(pan.pan(), -1.f);
  }
}

TEST(Algorithm_Pan_Mono, ShouldPanInput) {
  Clover::Stereo::PanMono<float> pan;
  float testSignal = 1.f;
  float midGain = Clover::Util::Calc::dbtol(-4.5f);

  pan.process(testSignal);
  EXPECT_FLOAT_EQ(pan.last()[0], midGain);
  EXPECT_FLOAT_EQ(pan.last()[1], midGain);

  pan.pan(1.f);
  pan.process(testSignal);

  EXPECT_FLOAT_EQ(pan.last()[0], 0.f);
  EXPECT_FLOAT_EQ(pan.last()[1], 1.f);

  pan.pan(-1.f);
  pan.process(testSignal);

  EXPECT_FLOAT_EQ(pan.last()[0], 1.f);
  EXPECT_FLOAT_EQ(pan.last()[1], 0.f);
}

TEST(Algorithm_Pan_Stereo, ShouldPanInput) {
  Clover::Stereo::PanStereo<float> pan;
  std::array<float, 2> testSignal{1.f, 1.f};
  float midGain = Clover::Util::Calc::dbtol(-4.5f);

  pan.process(testSignal);
  EXPECT_FLOAT_EQ(pan.last()[0], midGain);
  EXPECT_FLOAT_EQ(pan.last()[1], midGain);

  pan.pan(1.f);
  pan.process(testSignal);

  EXPECT_FLOAT_EQ(pan.last()[0], 0.f);
  EXPECT_FLOAT_EQ(pan.last()[1], 1.f);

  pan.pan(-1.f);
  pan.process(testSignal);

  EXPECT_FLOAT_EQ(pan.last()[0], 1.f);
  EXPECT_FLOAT_EQ(pan.last()[1], 0.f);
}

// migrate pan node test to nodesmoketest
