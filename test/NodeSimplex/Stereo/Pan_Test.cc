#include <gtest/gtest.h>

#include "Clover.h"

TEST(NodeSimplex_Stereo_Pan, ShouldPan1) {
  Clover::_Test::HandCrank<2> crank;
  Clover::_Test::Collector<2> collector(1);
  Clover::NodeSimplex::Stereo::Pan1 pan;
  Clover::_Test::DCN<1> dc;
  dc.indexBasis(1.f);

  dc >> pan >> collector >> crank;

  pan.pan(0);
  crank.turn(1);

  float midGain = Calc::dbtol(-4.5);
  EXPECT_EQ(collector.frames[0][0], midGain);
  EXPECT_EQ(collector.frames[0][1], midGain);

  pan.pan(1.);
  crank.turn(1);
  EXPECT_EQ(collector.frames[1][0], 0.f);
  EXPECT_EQ(collector.frames[1][1], 1.f);

  pan.pan(-1.);
  crank.turn(1);
  EXPECT_EQ(collector.frames[2][0], 1.f);
  EXPECT_EQ(collector.frames[2][1], 0.f);
}

TEST(NodeSimplex_Stereo_Pan, ShouldPan2) {
  Clover::_Test::HandCrank<2> crank;
  Clover::_Test::Collector<2> collector(1);
  Clover::NodeSimplex::Stereo::Pan2 pan;
  Clover::NodeSimplex::Envelope::DC dc;
  Clover::NodeSimplex::Adapter::MultiCast<2> multicast;
  dc.value(1);

  dc >> multicast >> pan >> collector >> crank;

  pan.pan(0);
  crank.turn(1);

  float midGain = Calc::dbtol(-4.5);
  EXPECT_EQ(collector.frames[0][0], midGain);
  EXPECT_EQ(collector.frames[0][1], midGain);

  pan.pan(1.);
  crank.turn(1);
  EXPECT_EQ(collector.frames[1][0], 0.f);
  EXPECT_EQ(collector.frames[1][1], 1.f);

  pan.pan(-1.);
  crank.turn(1);
  EXPECT_EQ(collector.frames[2][0], 1.f);
  EXPECT_EQ(collector.frames[2][1], 0.f);
}