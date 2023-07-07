#include <gtest/gtest.h>

#include "Algo/DynamicRange/Clip.h"

TEST(Algorithm_Dynamic_Range_Clip, ShouldClip) {
  Clover::DynamicRange::Clip<float, 1> clip;

  std::array<float, 1> below{0.5f};
  std::array<float, 1> at{1.f};
  std::array<float, 1> above{1.5f};

  std::array<float, 1> negative_below{-0.5f};
  std::array<float, 1> negative_at{-1.f};
  std::array<float, 1> negative_above{-1.5f};

  EXPECT_EQ(clip.process(below), below);
  EXPECT_EQ(clip.process(at), at);
  EXPECT_EQ(clip.process(above), at);

  EXPECT_EQ(clip.process(negative_above), negative_at);
  EXPECT_EQ(clip.process(negative_at), negative_at);
  EXPECT_EQ(clip.process(negative_below), negative_below);
}
