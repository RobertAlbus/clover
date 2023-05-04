#include <gtest/gtest.h>

#include "Clover.h"

TEST(Util_Calc, ShouldCalculateFreqDifferenceBySemitone) {
  EXPECT_EQ(Calc::freqBySemitoneDifference(1000, 12), 2000);
  EXPECT_EQ(Calc::freqBySemitoneDifference(1000, -12), 500);

  EXPECT_NEAR(Calc::freqBySemitoneDifference(1000.001, 12), 2000.002, 0.00005);
  EXPECT_NEAR(Calc::freqBySemitoneDifference(10000.001, 12), 20000.002, 0.00005);
}

TEST(Util_Calc, ShouldDetectSign) {
  EXPECT_EQ(Calc::sign(-10), -1);
  EXPECT_EQ(Calc::sign(0), 1);
  EXPECT_EQ(Calc::sign(10), 1);
}

TEST(Util_Calc, ShouldConvertDbToLinear) {
  EXPECT_EQ(Calc::dbtol(6.f), 1.99526238f);
  EXPECT_EQ(Calc::dbtol(-6), 0.5011872336f);
}

TEST(Util_Calc, ShouldConvertLinearToDb) {
  EXPECT_EQ(Calc::ltodb(2.f), 6.0205999133f);
  EXPECT_EQ(Calc::ltodb(0.5f), -6.0205999133f);
}

TEST(Util_Calc, ShouldConvertMidiToFreq) {
  EXPECT_EQ(Calc::mtof(0), 8.17579842f);
  EXPECT_EQ(Calc::mtof(63), 311.126983722f);
  EXPECT_EQ(Calc::mtof(127), 12543.8555f);
}

TEST(Util_Calc, ShouldConvertFreqToMidi) {
  EXPECT_NEAR(Calc::ftom(8.17579842f), 0, 0.00001);
  EXPECT_EQ(Calc::ftom(311.126983722f), 63);
  EXPECT_EQ(Calc::ftom(12543.8539514f), 127);
}

TEST(Util_Calc, ShouldHaveTensionCurve) {
  EXPECT_EQ(Calc::tension(0.5, 0), 0.5);

  EXPECT_NEAR(Calc::tension(0.5, -.297), 0.4, 0.066);
  EXPECT_EQ(Calc::tension(0.5, -10), 0.);

  EXPECT_NEAR(Calc::tension(0.5, 1.13343), 0.8, 0.000005);
  EXPECT_NEAR(Calc::tension(0.5, 10), 1., 0.00004);
}
