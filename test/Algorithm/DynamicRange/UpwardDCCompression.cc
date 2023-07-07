#include <gtest/gtest.h>

#include "Algo/DynamicRange/UpwardDCCompression.h"

TEST(Algorithm_Dynamic_Range_BipolarDCOffest, ShouldCompressUpwards) {
  Clover::DynamicRange::UpwardDCCompression<float, 1> avoider(0.1f);

  std::array<float, 1> value1{0.05f};
  std::array<float, 1> value2{0.5f};
  std::array<float, 1> value3{0.8f};

  EXPECT_GT(avoider.process(value1), value1);
  EXPECT_GT(avoider.process(value2), value2);
  EXPECT_GT(avoider.process(value3), value3);

  value1[0] = value1[0] * -1.f;
  value2[0] = value2[0] * -1.f;
  value3[0] = value3[0] * -1.f;

  EXPECT_LT(avoider.process(value1), value1);
  EXPECT_LT(avoider.process(value2), value2);
  EXPECT_LT(avoider.process(value3), value3);
}
