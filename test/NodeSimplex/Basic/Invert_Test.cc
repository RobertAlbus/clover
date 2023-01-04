#include <gtest/gtest.h>

#include "Clover.h"

TEST(NodeSimplex_Basic_Invert, ShouldInvert) {
  Clover::_Test::HandCrank<1> crank;
  Clover::_Test::Collector<1> collector(1);
  Clover::NodeSimplex::Basic::Invert<1> inverter;
  Clover::_Test::DCN<1> dc;

  dc >> inverter >> collector >> crank;
  dc.indexBasis(1);

  crank.turn(1);
  EXPECT_EQ(collector.frames[0][0], -1.f);
}