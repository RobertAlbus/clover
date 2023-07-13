
#include <gtest/gtest.h>

#include <vector>

#include "Algo/Wavetable/Generators.h"

TEST(Algorithm_Wavetable_Generator_Lerp, ShouldBeCorrectSize) {
  std::vector<float> rawTable{-1, 0, 0, 1};
  std::vector<float> lerpTable =
      Clover::Wavetable::LerpTable<float>(8, rawTable);

  EXPECT_EQ(Clover::Wavetable::LerpTable<float>(8, rawTable).size(), 8);
  EXPECT_EQ(Clover::Wavetable::LerpTable<float>(1024, rawTable).size(), 1024);
}

TEST(Algorithm_Wavetable_Generator_Lerp, ShouldLerp) {
  std::vector<float> rawTable{-1, 0, 0, 1};
  std::vector<float> lerpTable =
      Clover::Wavetable::LerpTable<float>(8, rawTable);

  EXPECT_EQ(lerpTable.size(), 8);

  EXPECT_FLOAT_EQ(lerpTable[0], -1.f);
  EXPECT_FLOAT_EQ(lerpTable[1], -0.5f);
  EXPECT_FLOAT_EQ(lerpTable[2], 0.f);
  EXPECT_FLOAT_EQ(lerpTable[3], 0.f);
  EXPECT_FLOAT_EQ(lerpTable[4], 0.f);
  EXPECT_FLOAT_EQ(lerpTable[5], 0.5f);
  EXPECT_FLOAT_EQ(lerpTable[6], 1.f);
  EXPECT_FLOAT_EQ(lerpTable[7], 0.5f);
}
