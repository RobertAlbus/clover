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

#include "Clover.h"

TEST(Graph_Frame, ShouldAdd) {
  Clover::Graph::AudioFrame<3> frame_1{1, 2, 3};
  Clover::Graph::AudioFrame<3> frame_2{1, 2, 3};

  frame_1 += frame_2;

  EXPECT_EQ(frame_1[0], 2.);
  EXPECT_EQ(frame_1[1], 4.);
  EXPECT_EQ(frame_1[2], 6.);
}

TEST(Graph_Frame, ShouldMultiply) {
  Clover::Graph::AudioFrame<3> frame_1{1, 2, 3};
  Clover::Graph::AudioFrame<3> frame_2{1, 2, 3};

  frame_1 *= frame_2;

  EXPECT_EQ(frame_1[0], 1.);
  EXPECT_EQ(frame_1[1], 4.);
  EXPECT_EQ(frame_1[2], 9.);

  frame_1 *= 2;

  EXPECT_EQ(frame_1[0], 2.);
  EXPECT_EQ(frame_1[1], 8.);
  EXPECT_EQ(frame_1[2], 18.);
}
