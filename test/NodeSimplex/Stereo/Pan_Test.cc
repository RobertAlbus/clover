#include <gtest/gtest.h>

#include <memory>

#include "Clover.h"

TEST(NodeSimplex_Stereo_Pan, ShouldPan1) {
  Clover::_Test::HandCrank<2> crank;
  Clover::_Test::Collector<2> collector(1);
  Clover::NodeSimplex::Stereo::Pan1 pan;
  Clover::_Test::DCN<1> dc;
  dc.indexBasis(1.f);

  dc >> pan >> collector >> crank;

  pan.pan(0.f);
  crank.turn(1);

  float midGain = Clover::Util::Calc::dbtol(-4.5f);
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
  std::shared_ptr<std::vector<float>> wt =
      std::make_shared<std::vector<float>>(1.f, 1.f);
  Clover::NodeSimplex::Wavetable::WavetableOscStereo wavetableDC;
  wavetableDC.wavetable(wt);

  Clover::_Test::HandCrank<2> crank;
  Clover::_Test::Collector<2> collector(1);
  Clover::NodeSimplex::Stereo::Pan2 pan;

  wavetableDC >> pan >> collector >> crank;

  pan.pan(0.f);
  crank.turn(1);

  float midGain = Clover::Util::Calc::dbtol(-4.5f);
  EXPECT_EQ(collector.frames[0][0], midGain);
  EXPECT_EQ(collector.frames[0][1], midGain);

  pan.pan(1.f);
  crank.turn(1.f);
  EXPECT_EQ(collector.frames[1][0], 0.f);
  // should re-evaluate the algorithm and try to make it equal exactly 1
  EXPECT_NEAR(collector.frames[1][1], 1.f, 0.01f);

  pan.pan(-1.f);
  crank.turn(1.f);
  // should re-evaluate the algorithm and try to make it equal exactly 1
  EXPECT_NEAR(collector.frames[2][0], 1.f, 0.01f);
  EXPECT_EQ(collector.frames[2][1], 0.f);
}