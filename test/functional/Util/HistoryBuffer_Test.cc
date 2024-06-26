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

#include "Clover/Util/HistoryBuffer.h"

TEST(Util_HistoryBuffer, ShouldHaveSize) {
  HistoryBuffer<float> buff(3);

  EXPECT_EQ(buff.size(), 3);
}

TEST(Util_HistoryBuffer, ShouldUpdate) {
  HistoryBuffer<float> buff(3);

  buff.tick(1.);
  EXPECT_EQ(buff.read(), 1.);
  buff.tick(2.);
  EXPECT_EQ(buff.read(), 2.);
  buff.tick(3.);
  EXPECT_EQ(buff.read(), 3.);
}

TEST(Util_HistoryBuffer, ShouldHaveHistory) {
  HistoryBuffer<float> buff(3);

  buff.tick(1.);
  buff.tick(2.);
  buff.tick(3.);
  EXPECT_EQ(buff.read(), 3.);
  EXPECT_EQ(buff.read(1), 2.);
  EXPECT_EQ(buff.read(2), 1.);
}
