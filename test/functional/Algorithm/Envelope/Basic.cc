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

#include "Clover/Algorithm/Envelope/Basic.h"

TEST(Algorithm_Envelope_Basic, ShouldInitializeAtZero) {
  Clover::Envelope::Basic<float> envelope;

  EXPECT_EQ(envelope.process(), 0.f);
  EXPECT_EQ(envelope.process(), 0.f);
}

TEST(Algorithm_Envelope_Basic, CanSetToAndFrom) {
  Clover::Envelope::Basic<float> envelope;

  envelope.set(0.f, 1.f, 5);

  EXPECT_EQ(envelope.process(), 0.0f);
  EXPECT_EQ(envelope.process(), 0.25f);
  EXPECT_EQ(envelope.process(), 0.5f);
  EXPECT_EQ(envelope.process(), 0.75f);
  EXPECT_EQ(envelope.process(), 1.f);

  envelope.set(1.f, 0.f, 5);

  EXPECT_EQ(envelope.process(), 1.f);
  EXPECT_EQ(envelope.process(), 0.75f);
  EXPECT_EQ(envelope.process(), 0.5f);
  EXPECT_EQ(envelope.process(), 0.25f);
  EXPECT_EQ(envelope.process(), 0.0f);
}

TEST(Algorithm_Envelope_Basic, CanSetTo) {
  Clover::Envelope::Basic<float> envelope;

  EXPECT_EQ(envelope.process(), 0.0f);

  envelope.set(1, 5);

  EXPECT_EQ(envelope.process(), 0.25f);
  EXPECT_EQ(envelope.process(), 0.5f);
  EXPECT_EQ(envelope.process(), 0.75f);
  EXPECT_EQ(envelope.process(), 1.f);

  envelope.set(0, 5);

  EXPECT_EQ(envelope.process(), 0.75f);
  EXPECT_EQ(envelope.process(), 0.5f);
  EXPECT_EQ(envelope.process(), 0.25f);
  EXPECT_EQ(envelope.process(), 0.0f);
}

TEST(Algorithm_Envelope_Basic, ShouldHoldTargetValue) {
  uint duration = 5;
  Clover::Envelope::Basic<float> envelope(0.f, 10.005f, 5);

  float result;
  for (uint i = 0; i < duration * 10u; i++) {
    result = envelope.process();
  }

  EXPECT_EQ(result, 10.005f);

  envelope.set(10.005f, 0.f, 5);

  for (uint i = 0; i < duration * 10u; i++) {
    result = envelope.process();
  }

  EXPECT_EQ(result, 0.f);
}
