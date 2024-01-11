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

#include "Algorithm/Math.h"
#include "Util/Calc.h"

TEST(Util_Calc, ShouldCalculateFreqDifferenceBySemitone) {
  // explicitly invoking the <float> template rather than appending `.f` for
  // template deduction
  EXPECT_EQ(
      Clover::Algorithm::frequency_by_semitone_difference<float>(1000, 12), 2000
  );
  EXPECT_EQ(
      Clover::Algorithm::frequency_by_semitone_difference<float>(1000, -12), 500
  );

  EXPECT_NEAR(
      Clover::Algorithm::frequency_by_semitone_difference<float>(1000.001, 12),
      2000.002,
      0.00005
  );
  EXPECT_NEAR(
      Clover::Algorithm::frequency_by_semitone_difference<float>(10000.001, 12),
      20000.002,
      0.00005
  );
}

TEST(Util_Calc, ShouldDetectSign) {
  EXPECT_EQ(Clover::Algorithm::sign(-10.f), -1.f);
  EXPECT_EQ(Clover::Algorithm::sign(0.f), 1.f);
  EXPECT_EQ(Clover::Algorithm::sign(10.f), 1.f);
}

TEST(Util_Calc, ShouldConvertDbToLinear) {
  EXPECT_EQ(Clover::Algorithm::db_to_linear(6.f), 1.99526238f);
  EXPECT_EQ(Clover::Algorithm::db_to_linear(-6.f), 0.5011872336f);
}

TEST(Util_Calc, ShouldConvertLinearToDb) {
  EXPECT_NEAR(Clover::Algorithm::linear_to_db(2.f), 6.0206f, 0.00001);
  EXPECT_NEAR(Clover::Algorithm::linear_to_db(0.5f), -6.0206, 0.00001);
}

TEST(Util_Calc, ShouldConvertMidiToFreq) {
  EXPECT_EQ(Clover::Util::Calc::mtof(0), 8.17579842f);
  EXPECT_EQ(Clover::Util::Calc::mtof(63), 311.126983722f);
  EXPECT_EQ(Clover::Util::Calc::mtof(127), 12543.8555f);
}

TEST(Util_Calc, ShouldConvertFreqToMidi) {
  EXPECT_NEAR(Clover::Util::Calc::ftom(8.17579842f), 0, 0.00001);
  EXPECT_EQ(Clover::Util::Calc::ftom(311.126983722f), 63);
  EXPECT_NEAR(Clover::Util::Calc::ftom(12543.8539514f), 127, 0.00001f);
}

TEST(Util_Calc, ShouldHaveTensionCurve) {
  EXPECT_EQ(Clover::Algorithm::tension(0.5f, 0.f), 0.5f);

  EXPECT_NEAR(Clover::Algorithm::tension(0.5f, -.297f), 0.4f, 0.066f);
  EXPECT_EQ(Clover::Algorithm::tension(0.5f, -10.f), 0.f);

  EXPECT_NEAR(Clover::Algorithm::tension(0.5f, 1.13343f), 0.72909f, 0.00001f);
  EXPECT_NEAR(Clover::Algorithm::tension(0.5f, 10.f), 0.99932f, 0.00003f);
}
