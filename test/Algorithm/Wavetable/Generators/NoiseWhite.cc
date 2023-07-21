
#include <gtest/gtest.h>

#include <vector>

#include "Algo/Wavetable/Generators.h"

TEST(Algorithm_Wavetable_Generator_NoiseWhite, ShouldBeCorrectSize) {
  EXPECT_EQ(Clover::Wavetable::NoiseWhite<float>(8).size(), 8);
  EXPECT_EQ(Clover::Wavetable::NoiseWhite<float>(1024).size(), 1024);
}

TEST(Algorithm_Wavetable_Generator_NoiseWhite, ShouldHaveCorrectRMS) {
  int size = 4096;
  std::vector<float> sineTable = Clover::Wavetable::NoiseWhite<float>(size);

  float squareAccumulator = 0;
  int numSamples = 10000;
  for (int i = 0; i < numSamples; i++) {
    float item = sineTable[i];
    squareAccumulator += item * item;
  }

  float rms = sqrt(squareAccumulator / static_cast<float>(numSamples));
  float expectedRms = 1.f / 3.f;

  EXPECT_NEAR(rms, expectedRms, 0.0333f);
}

TEST(Algorithm_Wavetable_Generator_NoiseWhite, ShouldBeWithinSignalBoundaries) {
  int size = 4096;
  std::vector<float> sineTable = Clover::Wavetable::NoiseWhite<float>(size);

  for (int i = 0, end = sineTable.size(); i < end; i++) {
    EXPECT_LE(sineTable[i], 1.f);
    EXPECT_GE(sineTable[i], -1.f);
  }
}

TEST(Algorithm_Wavetable_Generator_NoiseWhite, ShouldBeNormalized) {
  int size = 127;
  std::vector<float> table = Clover::Wavetable::NoiseWhite<float>(size);

  float absMaxima = 0.f;
  for (int i = 0, end = table.size(); i < end; i++) {
    if (fabs(table[i]) > absMaxima)
      absMaxima = table[i];
  }

  EXPECT_FLOAT_EQ(absMaxima, 1.f);
}
