
#include <gtest/gtest.h>

#include <vector>

#include "Algo/Wavetable/Generators.h"

TEST(Algorithm_Wavetable_Generator_Square, ShouldBeSquare) {
  float size = 4096;
  std::vector<float> squareTable = Clover::Wavetable::Square<float>(size);

  int halfTableSize = size / 2;
  EXPECT_FLOAT_EQ(squareTable[0], 1.f);
  EXPECT_FLOAT_EQ(squareTable[halfTableSize], 1.f);
  EXPECT_FLOAT_EQ(squareTable[halfTableSize + 1], -1.f);
  EXPECT_FLOAT_EQ(squareTable[size - 1], -1.f);
}
