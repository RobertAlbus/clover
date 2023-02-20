#include <gtest/gtest.h>

#include "Clover.h"

TEST(NodeSimplex_Stereo_Difference, ShouldPerformDifferenceCalculation) {
  Clover::_Test::HandCrank<2> crank;
  Clover::_Test::Collector<2> collector(1);
  Clover::NodeSimplex::Stereo::Difference difference;
  Clover::_Test::DCN<2> dc;

  dc >> difference >> collector >> crank;
  dc.indexBasis(1);

  crank.turn(1);
  EXPECT_EQ(collector.frames[0][0], -1.f);
  EXPECT_EQ(collector.frames[0][1], 1.f);
}