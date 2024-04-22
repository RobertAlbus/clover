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

#include "Clover/Algorithm/Delay/Fractional.h"

TEST(Algorithm_Delay_Fractional, ShouldInitializeWithZeroDelay) {
  Clover::Delay::FractionalDelay<float, 1> delay(512);

  for (int i = 0; i < 512; i++) {
    std::array<float, 1> f{i + 1.f};
    EXPECT_EQ(delay.process(f), f);
  }
}

TEST(Algorithm_Delay_Fractional, ShouldInitializeWithEmptyBuffer) {
  Clover::Delay::FractionalDelay<float, 1> delay(5, 5.f);

  std::array<float, 1> emptyFrame{0.f};

  for (int i = 0; i < 5; i++) {
    std::array<float, 1> f{i + 1.f};
    EXPECT_EQ(delay.process(f), emptyFrame);
  }

  std::array<float, 1> firstFrame{1.f};
  EXPECT_EQ(delay.process(emptyFrame), firstFrame);
}

TEST(Algorithm_Delay_Fractional, ShouldDelayFractionally) {
  Clover::Delay::FractionalDelay<float, 1> delay(512);
  delay.delay(0.5);

  std::array<float, 1> firstFrame{0};
  delay.process(firstFrame);

  for (int i = 0; i <= 512; i++) {
    std::array<float, 1> input{i + 1.f};
    std::array<float, 1> expectedOutput{
        (static_cast<float>(i + 1) + static_cast<float>(i)) / 2.F};
    EXPECT_EQ(delay.process(input), expectedOutput);
  }
}

TEST(Algorithm_Delay_Fractional, NegativeDelayTimeSaturatesAtZero) {
  Clover::Delay::FractionalDelay<float, 1> delay(512);
  delay.delay(-10.f);

  for (int i = 0; i < 512; i++) {
    std::array<float, 1> f{i + 1.f};
    EXPECT_EQ(delay.process(f), f);
  }
}

TEST(Algorithm_Delay_Fractional, ExcessiveDelayTimeSaturatesAtMax) {
  Clover::Delay::FractionalDelay<float, 1> delay(5);
  delay.delay(10.f);

  std::array<float, 1> emptyFrame{0};
  for (int i = 0; i < 5; i++) {
    std::array<float, 1> f{i + 1.f};
    EXPECT_EQ(delay.process(f), emptyFrame);
  }

  std::array<float, 1> firstFrame{1};
  EXPECT_EQ(delay.process(emptyFrame), firstFrame);
}

TEST(Algorithm_Delay_Fractional, LongDelayTimeWorks) {
  int fiveMinutes = 48000 * 60 * 5;
  Clover::Delay::FractionalDelay<float, 1> delay(fiveMinutes);
  delay.delay(static_cast<float>(fiveMinutes));

  std::array<float, 1> emptyFrame{0.f};
  std::array<float, 1> firstFrame{1.f};
  std::array<float, 1> secondFrame{2.f};

  for (long int i = 0; i < fiveMinutes; i++) {
    std::array<float, 1> f{i + 1.f};
    EXPECT_EQ(delay.process(f), emptyFrame);
  }

  EXPECT_EQ(delay.process(emptyFrame), firstFrame);
  EXPECT_EQ(delay.process(emptyFrame), secondFrame);
}
