#include <gtest/gtest.h>

#include "Clover.h"

TEST(NodeSimplex_Envelope_ADSR, ShouldOutputEnvelopes) {
  int phaseLength = 3333;

  Clover::_Test::HandCrank<1> crank;
  Clover::_Test::Collector<1> collector(phaseLength * 5);
  Clover::NodeSimplex::Envelope::AdsrSettings adsrSettings(
      phaseLength, phaseLength, 0.5, phaseLength);
  Clover::NodeSimplex::Envelope::Adsr adsr(adsrSettings);

  adsr >> collector >> crank;

  adsr.keyOn();
  crank.turn(phaseLength * 3);

  adsr.keyOff();
  crank.turn(phaseLength * 2);

  EXPECT_EQ(collector.frames[0][0], 0.f);
  EXPECT_EQ(collector.frames[phaseLength * 1][0], 1.f);
  EXPECT_EQ(collector.frames[phaseLength * 2][0], 0.5f);
  EXPECT_EQ(collector.frames[phaseLength * 3][0], 0.5f);
  EXPECT_EQ(collector.frames[phaseLength * 4][0], 0.f);
}

TEST(NodeSimplex_Envelope_ADSR, ShouldScaleOutputWithGain) {
  int phaseLength = 3333;

  Clover::_Test::HandCrank<1> crank;
  Clover::_Test::Collector<1> collector(phaseLength * 5);
  Clover::NodeSimplex::Envelope::AdsrSettings adsrSettings(
      phaseLength, phaseLength, 0.5, phaseLength);
  Clover::NodeSimplex::Envelope::Adsr adsr(adsrSettings);
  adsr >> collector >> crank;

  adsr.gain(2);

  adsr.keyOn();
  crank.turn(phaseLength * 3);

  adsr.keyOff();
  crank.turn(phaseLength * 2);

  EXPECT_EQ(collector.frames[0][0], 0.f);
  EXPECT_EQ(collector.frames[phaseLength * 1][0], 2.f);
  EXPECT_EQ(collector.frames[phaseLength * 2][0], 1.f);
  EXPECT_EQ(collector.frames[phaseLength * 3][0], 1.f);
  EXPECT_EQ(collector.frames[phaseLength * 4][0], 0.f);
}