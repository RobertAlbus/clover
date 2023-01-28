#include <gtest/gtest.h>

#include "Clover.h"

TEST(NodeSimplex_Envelope_DC, ShouldOutputDC) {
  Clover::_Test::HandCrank<1> crank;
  Clover::_Test::Collector<1> collector(100);
  Clover::NodeSimplex::Envelope::DC dc;

  dc.value(1.);

  dc >> collector >> crank;

  // default gain should be 1.f
  crank.turn(100);
  EXPECT_EQ(collector.frames[0][0], 1.f);
  EXPECT_EQ(collector.frames[99][0], 1.f);
}