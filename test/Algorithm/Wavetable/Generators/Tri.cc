
#include <gtest/gtest.h>

#include <vector>

#include "Algo/Wavetable/Generators.h"

TEST(Algorithm_Wavetable_Generator_Tri, ShouldBeTriangle) {
  std::vector<float> table5 = Clover::Wavetable::Tri<float>(5);

  EXPECT_FLOAT_EQ(table5[0], 0.f);
  EXPECT_FLOAT_EQ(table5[1], 1.f);
  EXPECT_FLOAT_EQ(table5[2], 0.f);
  EXPECT_FLOAT_EQ(table5[3], -1.f);
  EXPECT_FLOAT_EQ(table5[4], 0.f);

  std::vector<float> table103 = Clover::Wavetable::Tri<float>(103);

  EXPECT_FLOAT_EQ(table103[0], 0.f);
  EXPECT_FLOAT_EQ(table103[25], 1.f);
  EXPECT_FLOAT_EQ(table103[51], 0.f);
  EXPECT_FLOAT_EQ(table103[76], -1.f);
  EXPECT_FLOAT_EQ(table103[102], 0.f);
}

TEST(Algorithm_Wavetable_Generator_Tri, ShouldBeNormalized) {
  int size = 88;
  std::vector<float> table = Clover::Wavetable::Tri<float>(size);

  float absMaxima = 0.f;
  for (int i = 0; i < size; i++) {
    float abs = fabsf(table[i]);
    if (abs > absMaxima)
      absMaxima = abs;
  }

  EXPECT_FLOAT_EQ(absMaxima, 1.f);
}
