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

#include "Clover/Algorithm/Math.h"

TEST(Algorithm_Math, ShouldCalculateFreqDifferenceBySemitone) {
  EXPECT_EQ(
      Clover::Algorithm::frequency_by_semitone_difference(1000.f, 12.f), 2000
  );
  EXPECT_EQ(
      Clover::Algorithm::frequency_by_semitone_difference(1000.f, -12.f), 500
  );

  EXPECT_NEAR(
      Clover::Algorithm::frequency_by_semitone_difference(1000.001f, 12.f),
      2000.002,
      0.00005
  );
  EXPECT_NEAR(
      Clover::Algorithm::frequency_by_semitone_difference(10000.001f, 12.f),
      20000.002,
      0.00005
  );
}

TEST(Algorithm_Math, ShouldDetectSign) {
  EXPECT_EQ(Clover::Algorithm::sign(-10.f), -1.f);
  EXPECT_EQ(Clover::Algorithm::sign(0.f), 1.f);
  EXPECT_EQ(Clover::Algorithm::sign(10.f), 1.f);
}

TEST(Algorithm_Math, ShouldConvertDbToLinear) {
  EXPECT_EQ(Clover::Algorithm::db_to_linear(6.f), 1.99526238f);
  EXPECT_EQ(Clover::Algorithm::db_to_linear(-6.f), 0.5011872336f);
}

TEST(Algorithm_Math, ShouldConvertLinearToDb) {
  EXPECT_NEAR(Clover::Algorithm::linear_to_db(2.f), 6.0206f, 0.00001);
  EXPECT_NEAR(Clover::Algorithm::linear_to_db(0.5f), -6.0206, 0.00001);
}

TEST(Algorithm_Math, ShouldConvertMidiToFreq) {
  EXPECT_NEAR(
      Clover::Algorithm::midi_to_frequency<float>(0), 8.175798f, 0.000001f
  );
  EXPECT_EQ(Clover::Algorithm::midi_to_frequency<float>(63), 311.126983722f);
  EXPECT_EQ(Clover::Algorithm::midi_to_frequency<float>(127), 12543.8555f);
}

TEST(Algorithm_Math, ShouldConvertFreqToMidi) {
  EXPECT_NEAR(Clover::Algorithm::frequency_to_midi(8.17579842f), 0, 0.00001);
  EXPECT_EQ(Clover::Algorithm::frequency_to_midi(311.126983722f), 63);
  EXPECT_NEAR(
      Clover::Algorithm::frequency_to_midi(12543.8539514f), 127, 0.00001f
  );
}

TEST(Algorithm_Math, ShouldHaveTensionCurve) {
  EXPECT_EQ(Clover::Algorithm::tension(0.5f, 0.f), 0.5f);

  EXPECT_NEAR(Clover::Algorithm::tension(0.5f, -.297f), 0.4f, 0.066f);
  EXPECT_EQ(Clover::Algorithm::tension(0.5f, -10.f), 0.f);

  EXPECT_NEAR(Clover::Algorithm::tension(0.5f, 1.13343f), 0.72909f, 0.00001f);
  EXPECT_NEAR(Clover::Algorithm::tension(0.5f, 10.f), 0.99932f, 0.00003f);
}
