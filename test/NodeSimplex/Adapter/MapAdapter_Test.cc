#include <gtest/gtest.h>

#include "Clover.h"

TEST(NodeSimplex_Adapter_MapAdapter, ShouldMapNone) {
  Clover::_Test::HandCrank<4> crank;
  Clover::_Test::Collector<4> collector(1);
  Clover::NodeSimplex::Adapter::MapAdapter<4, 4> mapAdapter;
  Clover::_Test::DCN<4> dc;

  dc >> mapAdapter >> collector >> crank;

  crank.turn(1);

  EXPECT_EQ(collector.frames[0][0], 0.f);
  EXPECT_EQ(collector.frames[0][1], 0.f);
  EXPECT_EQ(collector.frames[0][2], 0.f);
  EXPECT_EQ(collector.frames[0][3], 0.f);
}

TEST(NodeSimplex_Adapter_MapAdapter, ShouldMapSingle) {
  Clover::_Test::HandCrank<4> crank;
  Clover::_Test::Collector<4> collector(1);
  Clover::NodeSimplex::Adapter::MapAdapter<4, 4> mapAdapter;
  Clover::_Test::DCN<4> dc;

  dc >> mapAdapter >> collector >> crank;

  mapAdapter.map(1, 1);
  mapAdapter.map(2, 3);
  crank.turn(1);

  EXPECT_EQ(collector.frames[0][0], 0.f);
  EXPECT_EQ(collector.frames[0][1], 1.f);
  EXPECT_EQ(collector.frames[0][2], 0.f);
  EXPECT_EQ(collector.frames[0][3], 2.f);
}

TEST(NodeSimplex_Adapter_MapAdapter, ShouldMapRange) {
  Clover::_Test::HandCrank<4> crank;
  Clover::_Test::Collector<4> collector(1);
  Clover::NodeSimplex::Adapter::MapAdapter<4, 4> mapAdapter;
  Clover::_Test::DCN<4> dc;

  dc >> mapAdapter >> collector >> crank;

  mapAdapter.mapRange(2, 2, 1);
  crank.turn(1);

  EXPECT_EQ(collector.frames[0][0], 0.f);
  EXPECT_EQ(collector.frames[0][1], 2.f);
  EXPECT_EQ(collector.frames[0][2], 3.f);
  EXPECT_EQ(collector.frames[0][3], 0.f);
}

// test for out of bounds mapping