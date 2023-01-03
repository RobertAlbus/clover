#include <gtest/gtest.h>

#include "Clover.h"

TEST(NodeSimplex_Adapter_NullAdapter, ShouldMapNull) {
  Clover::_Test::HandCrank<1> crank;
  Clover::_Test::Collector<1> collector(1);
  Clover::NodeSimplex::Adapter::NullAdapter<4, 1> nullAdapter;
  Clover::_Test::DCN<4> dc;

  dc >> nullAdapter >> collector >> crank;

  crank.turn(1);

  EXPECT_EQ(collector.frames[0][0], 0.f);
}