#include <gtest/gtest.h>

#include "Clover.h"

TEST(NodeSimplex_Basic_Gain, ShouldNotModify) {
  Clover::_Test::HandCrank crank;
  Clover::_Test::Collector<1> collector(1);
  Clover::NodeSimplex::Basic::Gain<1> g;
  Clover::_Test::DCN<1> dc;

  dc >> g >> collector >> crank;
  dc.indexBasis(1);

  // default gain should be 1.f
  crank.turn(1);
  EXPECT_EQ(collector.frames[0][0], 1.f);

  g.gain(1.f);
  crank.turn(1);
  EXPECT_EQ(collector.frames[1][0], 1.f);
}

TEST(NodeSimplex_Basic_Gain, ShouldModify) {
  Clover::_Test::HandCrank crank;
  Clover::_Test::Collector<1> collector(3);
  Clover::NodeSimplex::Basic::Gain<1> g;
  Clover::_Test::DCN<1> dc;

  dc >> g >> collector >> crank;
  dc.indexBasis(1);

  g.gain(2.f);
  crank.turn(1);
  EXPECT_EQ(collector.frames[0][0], 2.f);

  g.gain(0.5f);
  crank.turn(1);
  EXPECT_EQ(collector.frames[1][0], 0.5f);

  // invert
  g.gain(-1.f);
  crank.turn(1);
  EXPECT_EQ(collector.frames[2][0], -1.f);
}