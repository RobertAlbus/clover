#include <gtest/gtest.h>

#include "Algo/DynamicRange/AsymClip.h"

TEST(Algorithm_Dynamic_Range_Clip, ShouldInitialize) {
  Clover::DynamicRange::AsymClip<float, 1> clip;

  EXPECT_EQ(clip.min(), -1.f);
  EXPECT_EQ(clip.max(), 1.f);
}

TEST(Algorithm_Dynamic_Range_Clip, ShouldClipAsymetrically) {
  Clover::DynamicRange::AsymClip<float, 1> clip;
  clip.min(-0.9f);
  clip.max(0.95f);

  std::array<float, 1> within{0.5f};
  std::array<float, 1> at{0.95f};
  std::array<float, 1> exceed{1.5f};

  std::array<float, 1> negative_within{-0.5f};
  std::array<float, 1> negative_at{-0.9f};
  std::array<float, 1> negative_exceed{-1.5f};

  EXPECT_EQ(clip.process(within), within);
  EXPECT_EQ(clip.process(at), at);
  EXPECT_EQ(clip.process(exceed), at);

  EXPECT_EQ(clip.process(negative_exceed), negative_at);
  EXPECT_EQ(clip.process(negative_at), negative_at);
  EXPECT_EQ(clip.process(negative_within), negative_within);
}
