#include <gtest/gtest.h>

#include "Clover.h"

TEST(Graph_FrameHistory, ShouldInitialize) {
  FrameHistory<AudioFrame<1>> frameHistory;

  EXPECT_EQ(frameHistory.current[0], 0.);
  EXPECT_EQ(frameHistory.last[0], 0.);
}

TEST(Graph_FrameHistory, ShouldBeNextable) {
  AudioFrame<1> frame_1{1};
  AudioFrame<1> frame_2{2};
  AudioFrame<1> frame_3{3};
  AudioFrame<1> frame_4{4};
  AudioFrame<1> frame_5{5};

  FrameHistory<AudioFrame<1>> frameHistory;

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
