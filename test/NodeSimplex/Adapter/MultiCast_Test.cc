#include <gtest/gtest.h>

#include "Clover.h"

TEST(NodeSimplex_Adapter_MultiCast, ShouldMulticast) {
  Clover::_Test::HandCrank<4> crank;
  Clover::_Test::Collector<4> collector(1);
  Clover::NodeSimplex::Adapter::MultiCast<4> multiCast;
  Clover::_Test::DCN<1> dc;

  dc.indexBasis(1.);

  dc >> multiCast >> collector >> crank;

  crank.turn(1);

  EXPECT_EQ(collector.frames[0][0], 1.f);
  EXPECT_EQ(collector.frames[0][1], 1.f);
  EXPECT_EQ(collector.frames[0][2], 1.f);
  EXPECT_EQ(collector.frames[0][3], 1.f);
}