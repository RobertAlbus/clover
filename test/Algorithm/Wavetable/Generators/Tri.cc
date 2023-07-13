
#include <gtest/gtest.h>

#include <vector>

#include "Algo/Wavetable/Generators.h"

TEST(Algorithm_Wavetable_Generator_Tri, ShouldBeTriangle) {
  float size = 5;
  std::vector<float> table = Clover::Wavetable::Tri<float>(size);

  int halfTableSize = size / 2;
  EXPECT_FLOAT_EQ(table[0], 0.f);
  EXPECT_FLOAT_EQ(table[1], 1.f);
  EXPECT_FLOAT_EQ(table[2], 0.f);
  EXPECT_FLOAT_EQ(table[3], -1.f);
  EXPECT_FLOAT_EQ(table[4], 0.f);
}
