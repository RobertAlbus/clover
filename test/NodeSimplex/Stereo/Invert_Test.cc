#include <gtest/gtest.h>

#include "Clover.h"

TEST(NodeSimplex_Stereo_Invert, ShouldInvertChannels) {
  Clover::_Test::HandCrank<2> crank;
  Clover::_Test::Collector<2> collector(1);
  Clover::NodeSimplex::Stereo::Invert invert;
  Clover::_Test::DCN<2> dc;

  dc >> invert >> collector >> crank;

  crank.turn(1);
  EXPECT_EQ(collector.frames[0][0], 1.f);
  EXPECT_EQ(collector.frames[0][1], 0.f);
}