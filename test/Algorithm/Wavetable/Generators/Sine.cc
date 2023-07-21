
#include <gtest/gtest.h>

#include <vector>

#include "Algo/Wavetable/Generators.h"

TEST(Algorithm_Wavetable_Generator_Sine, ShouldBeCorrectSize) {
  EXPECT_EQ(Clover::Wavetable::Sine<float>(8).size(), 8);
  EXPECT_EQ(Clover::Wavetable::Sine<float>(1024).size(), 1024);
}

TEST(Algorithm_Wavetable_Generator_Sine, ShouldHaveCorrectRMS) {
  int size = 4096;
  std::vector<float> sineTable = Clover::Wavetable::Sine<float>(size);

  float squareAccumulator = 0;
  for (int i = 0, end = sineTable.size(); i < end; i++) {
    float item = sineTable[i];
    squareAccumulator += item * item;
  }

  float rms = sqrt(squareAccumulator / static_cast<float>(sineTable.size()));
  float expectedRms = 1.f / sqrtf(2.f);

  EXPECT_FLOAT_EQ(rms, expectedRms);
}

TEST(Algorithm_Wavetable_Generator_Sine, ShouldBeWithinSignalBoundaries) {
  int size = 4096;
  std::vector<float> sineTable = Clover::Wavetable::Sine<float>(size);

  for (int i = 0, end = sineTable.size(); i < end; i++) {
    EXPECT_LE(sineTable[i], 1.f);
    EXPECT_GE(sineTable[i], -1.f);
  }
}

TEST(Algorithm_Wavetable_Generator_Sine, ShouldBeNormalized) {
  // a table size of 127 results in minima/maxima +/- 0.99992353
  int size = 127;
  std::vector<float> table = Clover::Wavetable::Sine<float>(size);

  float absMaxima = 0.f;
  for (int i = 0, end = table.size(); i < end; i++) {
    if (fabs(table[i]) > absMaxima)
      absMaxima = table[i];
  }

  EXPECT_FLOAT_EQ(absMaxima, 1.f);
}
