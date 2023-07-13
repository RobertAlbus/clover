
#include <gtest/gtest.h>

#include <vector>

#include "Algo/Wavetable/Generators.h"

TEST(Algorithm_Wavetable_Generator_Saw, ShouldBeCorrectSize) {
  EXPECT_EQ(Clover::Wavetable::Saw<float>(8).size(), 8);
  EXPECT_EQ(Clover::Wavetable::Saw<float>(1024).size(), 1024);
}

TEST(Algorithm_Wavetable_Generator_Saw, ShouldMoveHighToLow) {
  int tableSize = 4096;
  std::vector<float> table = Clover::Wavetable::Saw<float>(tableSize);

  EXPECT_FLOAT_EQ(table[0], 1.f);
  EXPECT_FLOAT_EQ(table[tableSize - 1], -1.f);

  for (int i = 1; i < tableSize; i++) {
    EXPECT_LT(table[i], table[i - 1]) << i;
  }
}

TEST(Algorithm_Wavetable_Generator_Saw, ShouldBeWithinSignalBoundaries) {
  int tableSize = 4096;
  std::vector<float> table = Clover::Wavetable::Saw<float>(tableSize);

  for (int i = 0; i < tableSize; i++) {
    EXPECT_LE(table[i], 1.f);
    EXPECT_GE(table[i], -1.f);
  }
}
