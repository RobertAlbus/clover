#include <gtest/gtest.h>

#include <array>

#include "Algo/Stereo/Sum.h"

TEST(Algorithm_Sum_Mono, ShouldPerformSumCalculation) {
  Clover::Stereo::SumMono<float> sum;

  // sum to null
  std::array<float, 2> frame{1.f, -1.f};
  float result = sum.process(frame);

  EXPECT_FLOAT_EQ(result, 0.f);

  // sum to mono to mono
  frame[0] = -1.f;
  frame[1] = -1.f;
  result = sum.process(frame);

  EXPECT_FLOAT_EQ(result, -1.f);

  // sum stereo to mono
  frame[0] = 1.f;
  frame[1] = 0.5f;
  result = sum.process(frame);

  EXPECT_FLOAT_EQ(result, 0.75f);
}

TEST(Algorithm_Sum_Stereo, ShouldPerformSumCalculation) {
  Clover::Stereo::SumStereo<float> sum;

  // sum to null
  std::array<float, 2> frame{1.f, -1.f};
  std::array<float, 2> result = sum.process(frame);

  EXPECT_FLOAT_EQ(result[0], 0.f);
  EXPECT_FLOAT_EQ(result[1], 0.f);

  // sum to mono to mono
  frame[0] = -1.f;
  frame[1] = -1.f;
  result = sum.process(frame);

  EXPECT_FLOAT_EQ(result[0], -1.f);
  EXPECT_FLOAT_EQ(result[1], -1.f);

  // sum stereo to mono
  frame[0] = 1.f;
  frame[1] = 0.5f;
  result = sum.process(frame);

  EXPECT_FLOAT_EQ(result[0], 0.75f);
  EXPECT_FLOAT_EQ(result[1], 0.75f);
}