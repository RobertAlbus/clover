#include <gtest/gtest.h>

#include "Clover.h"

TEST(NodeSimplex_Envelope_Basic, ShouldChangeOverTime) {
  int numFramesToTest = 48000 * 10;
  int envelopeLength = numFramesToTest - 10;
  float upperBound = 1.;
  float LowerBound = 0.;

  Clover::_Test::HandCrank<1> crank;
  Clover::_Test::Collector<1> collector(numFramesToTest);
  Clover::NodeSimplex::Envelope::BasicEnvelope envelope(LowerBound, upperBound,
                                                        envelopeLength);

  envelope >> collector >> crank;

  crank.turn(numFramesToTest);

  EXPECT_EQ(collector.frames[0][0], 0.f)
      << "Envelope should start at the correct value one tick later";
  EXPECT_EQ(collector.frames[envelopeLength / 2][0], 0.5f)
      << "Envelope should be linear";
  EXPECT_EQ(collector.frames[envelopeLength][0], 1.f)
      << "Envelope should reach target value on time";
  EXPECT_EQ(collector.frames[numFramesToTest - 1][0], 1.f)
      << "Envelope should maintain target value indefinitely";

  bool exceededLowerBound = false;
  bool exceededUpperBound = false;
  for (int i = 0; i < numFramesToTest; i++) {
    if (collector.frames[i][0] < LowerBound) {
      exceededLowerBound = true;
      break;
    }
    if (collector.frames[i][0] > upperBound) {
      exceededUpperBound = true;
      break;
    }
  }

  EXPECT_FALSE(exceededLowerBound) << "Exceeded lower bound of envelope range";
  EXPECT_FALSE(exceededUpperBound) << "Exceeded upper bound of envelope range";
}