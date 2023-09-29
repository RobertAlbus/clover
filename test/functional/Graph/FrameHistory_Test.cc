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

#include "Graph/AudioFrame.h"
#include "Graph/FrameHistory.h"

TEST(Graph_FrameHistory, ShouldInitialize) {
  Clover::Graph::FrameHistory<Clover::Graph::AudioFrame<1>> frameHistory;

  EXPECT_EQ(frameHistory.current[0], 0.);
  EXPECT_EQ(frameHistory.last[0], 0.);
}

TEST(Graph_FrameHistory, ShouldBeNextable) {
  Clover::Graph::AudioFrame<1> frame_1{1};
  Clover::Graph::AudioFrame<1> frame_2{2};
  Clover::Graph::AudioFrame<1> frame_3{3};
  Clover::Graph::AudioFrame<1> frame_4{4};
  Clover::Graph::AudioFrame<1> frame_5{5};

  Clover::Graph::FrameHistory<Clover::Graph::AudioFrame<1>> frameHistory;

  // can push
  frameHistory.push(frame_1);
  EXPECT_EQ(frameHistory.current[0], frame_1[0]);

  // can override the next push (does not advance yet)
  frameHistory.next(frame_2);
  EXPECT_EQ(frameHistory.current[0], frame_1[0]);

  // next push is overridden
  frameHistory.push(frame_3);
  EXPECT_EQ(frameHistory.current[0], frame_2[0]);
  EXPECT_EQ(frameHistory.last[0], frame_1[0]);

  // can override more than once, with recency precedence
  frameHistory.next(frame_4);
  frameHistory.next(frame_5);
  frameHistory.push(frame_3);
  EXPECT_EQ(frameHistory.current[0], frame_5[0]);
  EXPECT_EQ(frameHistory.last[0], frame_2[0]);
}
