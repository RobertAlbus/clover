#include <gtest/gtest.h>

#include "Clover.h"

TEST(NodeSimplex_Basic_Rectifier, ShouldRectify) {
  Clover::_Test::HandCrank<3> crank;
  Clover::_Test::Collector<3> collector(1);
  Clover::NodeSimplex::Basic::Rectifier<3> rectifier;
  Clover::_Test::DCN<3> dc;

  dc >> rectifier >> collector >> crank;
  dc.indexBasis(-1);

  crank.turn(1);
  EXPECT_EQ(collector.frames[0][0], 1.f);
  EXPECT_EQ(collector.frames[0][1], 0.f);
  EXPECT_EQ(collector.frames[0][2], 1.f);
}