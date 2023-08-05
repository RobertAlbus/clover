#include <gtest/gtest.h>

#include <array>

#include "Algo/Stereo/MidSideBalance.h"

TEST(Algorithm_Stereo_Spread, ShouldInitialize) {
  Clover::Stereo::MidSideBalance<float> defaultConstructor;
  EXPECT_FLOAT_EQ(defaultConstructor.spread(), 0.f);

  Clover::Stereo::MidSideBalance<float> explicitConstructor(-1.f);
  EXPECT_FLOAT_EQ(explicitConstructor.spread(), -1.f);
}

TEST(Algorithm_Stereo_Spread, ShouldSpreadInvertedSignal) {
  Clover::Stereo::MidSideBalance<float> image;

  std::array<float, 2> frame{1.f, -1.f};

  image.spread(-1.f);
  image.process(frame);

  EXPECT_FLOAT_EQ(image.last()[0], 0.f);
  EXPECT_FLOAT_EQ(image.last()[1], 0.f);

  image.spread(1.f);
  image.process(frame);

  EXPECT_FLOAT_EQ(image.last()[0], 1.f);
  EXPECT_FLOAT_EQ(image.last()[1], -1.f);
}

TEST(Algorithm_Stereo_Spread, ShouldNotSpreadMonoSignal) {
  Clover::Stereo::MidSideBalance<float> image;

  std::array<float, 2> frame{1.f, 1.f};

  image.spread(-1.f);
  image.process(frame);

  EXPECT_FLOAT_EQ(image.last()[0], 1.f);
  EXPECT_FLOAT_EQ(image.last()[1], 1.f);

  image.spread(1.f);
  image.process(frame);

  EXPECT_FLOAT_EQ(image.last()[0], 0.f);
  EXPECT_FLOAT_EQ(image.last()[1], 0.f);
}

TEST(Algorithm_Stereo_Spread, ShouldSpreadStereoSignal) {
  Clover::Stereo::MidSideBalance<float> image;

  std::array<float, 2> frame{1.f, 0.9f};

  image.spread(-1.f);
  image.process(frame);

  EXPECT_FLOAT_EQ(image.last()[0], 0.95);
  EXPECT_FLOAT_EQ(image.last()[1], 0.95);

  image.spread(1.f);
  image.process(frame);

  EXPECT_FLOAT_EQ(image.last()[0], 0.05f);
  EXPECT_FLOAT_EQ(image.last()[1], -0.05f);
}
