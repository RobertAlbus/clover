#include <gtest/gtest.h>

#include <array>

#include "Algo/Stereo/Difference.h"

TEST(Algorithm_Stereo_Difference, ShouldPerformDifferenceCalculation) {
  Clover::Stereo::Difference<float> difference;

  std::array<float, 2> frame{1.f, -1.f};
  std::array<float, 2> result = difference.process(frame);

  EXPECT_FLOAT_EQ(result[0], 1.f);
  EXPECT_FLOAT_EQ(result[1], -1.f);

  frame[0] = 1.f;
  frame[1] = 0.5f;
  result = difference.process(frame);

  EXPECT_FLOAT_EQ(result[0], 0.25f);
  EXPECT_FLOAT_EQ(result[1], -0.25f);

  frame[0] = -1.f;
  frame[1] = -1.f;
  result = difference.process(frame);

  EXPECT_FLOAT_EQ(result[0], 0.f);
  EXPECT_FLOAT_EQ(result[1], 0.f);
}