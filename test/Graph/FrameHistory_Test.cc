#include <gtest/gtest.h>

#include "Clover.h"

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
