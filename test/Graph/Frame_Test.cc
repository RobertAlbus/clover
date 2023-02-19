#include <gtest/gtest.h>

#include "Clover.h"

TEST(Graph_Frame, ShouldAdd) {
  Frame<3> frame_1{1, 2, 3};
  Frame<3> frame_2{1, 2, 3};

  frame_1 += frame_2;

  EXPECT_EQ(frame_1[0], 2.);
  EXPECT_EQ(frame_1[1], 4.);
  EXPECT_EQ(frame_1[2], 6.);
}

TEST(Graph_Frame, ShouldMultiply) {
  Frame<3> frame_1{1, 2, 3};
  Frame<3> frame_2{1, 2, 3};

  frame_1 *= frame_2;

  EXPECT_EQ(frame_1[0], 1.);
  EXPECT_EQ(frame_1[1], 4.);
  EXPECT_EQ(frame_1[2], 9.);

  frame_1 *= 2;

  EXPECT_EQ(frame_1[0], 2.);
  EXPECT_EQ(frame_1[1], 8.);
  EXPECT_EQ(frame_1[2], 18.);
}
