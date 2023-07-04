#include <gtest/gtest.h>

#include "Clover.h"

TEST(Algorithm_Envelope_DC, ShouldInitializeAtZero) {
  Clover::Envelope::DC<float> dc;

  EXPECT_EQ(dc.process(), 0.f);
  EXPECT_EQ(dc.process(), 0.f);
  EXPECT_EQ(dc.process(), 0.f);
}

TEST(Algorithm_Envelope_DC, ShouldOutputDC) {
  Clover::Envelope::DC<float> dc(1.);

  EXPECT_EQ(dc.process(), 1.f);
  EXPECT_EQ(dc.process(), 1.f);
  EXPECT_EQ(dc.process(), 1.f);

  dc.value(2.);

  EXPECT_EQ(dc.process(), 2.f);
  EXPECT_EQ(dc.process(), 2.f);
  EXPECT_EQ(dc.process(), 2.f);
}