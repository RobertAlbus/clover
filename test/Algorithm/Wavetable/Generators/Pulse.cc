
#include <gtest/gtest.h>

#include <vector>

#include "Algo/Wavetable/Generators.h"

TEST(Algorithm_Wavetable_Generator_Pulse, ShouldBeCorrectSize) {
  EXPECT_EQ(Clover::Wavetable::Pulse<float>(8, 0.25f).size(), 8);
  EXPECT_EQ(Clover::Wavetable::Pulse<float>(1024, 0.5f).size(), 1024);
}

TEST(Algorithm_Wavetable_Generator_Pulse, ShouldHaveCorrectProportions) {
  int tableSize = 4096;
  float pulseWidth = 0.25f;
  std::vector<float> table =
      Clover::Wavetable::Pulse<float>(tableSize, pulseWidth);

  int positiveRegion = tableSize * pulseWidth;
  for (int i = 0; i <= positiveRegion; i++) {
    EXPECT_FLOAT_EQ(table[i], 1.f) << i;
  }
  for (int i = positiveRegion + 1; i < tableSize; i++) {
    EXPECT_FLOAT_EQ(table[i], -1.f) << i;
  }
}

TEST(Algorithm_Wavetable_Generator_Pulse, ShouldBeWithinSignalBoundaries) {
  int tableSize = 4096;
  float pulseWidth = 0.25f;
  std::vector<float> table =
      Clover::Wavetable::Pulse<float>(tableSize, pulseWidth);

  for (int i = 0; i < tableSize; i++) {
    EXPECT_LE(table[i], 1.f);
    EXPECT_GE(table[i], -1.f);
  }
}

TEST(Algorithm_Wavetable_Generator_Pulse, ShouldBeNormalized) {
  int tableSize = 4096;
  float pulseWidth = 0.25f;
  std::vector<float> table =
      Clover::Wavetable::Pulse<float>(tableSize, pulseWidth);

  float absMaxima = 0.f;
  for (int i = 0, end = table.size(); i < end; i++) {
    if (fabs(table[i]) > absMaxima)
      absMaxima = table[i];
  }

  EXPECT_FLOAT_EQ(absMaxima, 1.f);
}
