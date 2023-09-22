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

#include "Algo/Envelope/DC.h"

TEST(Algorithm_Envelope_DC, ShouldInitializeAtZero) {
  Clover::Envelope::DC<float> dc;

  EXPECT_EQ(dc.process(), 0.f);
  EXPECT_EQ(dc.process(), 0.f);
  EXPECT_EQ(dc.process(), 0.f);
}

TEST(Algorithm_Envelope_DC, ShouldOutputDC) {
  Clover::Envelope::DC<float> dc(1.);

  EXPECT_EQ(dc.process(), 1.f);
  EXPECT_EQ(dc.process(), 1.f);
  EXPECT_EQ(dc.process(), 1.f);

  dc.value(2.);

  EXPECT_EQ(dc.process(), 2.f);
  EXPECT_EQ(dc.process(), 2.f);
  EXPECT_EQ(dc.process(), 2.f);
}
