#include <gtest/gtest.h>

#include <cmath>
#include <limits>

#include "Clover.h"

TEST(NodeSimplex_Filter_Filter, ShouldFilterLowPass) {

  int testSize = 10000;
  Clover::_Test::HandCrank crank;
  Clover::_Test::Collector<1> collector(testSize);
  Clover::NodeSimplex::Filter::Filter<1> filter;
  Clover::NodeSimplex::Wavetable::WavetableOsc osc;

  osc >> filter >> collector >> crank;

  osc.sine();
  osc.freq(20000);

  filter.lowPass();
  filter.set(100, 10);

  crank.turn(testSize);

  float rms = 0;
  for (Clover::Graph::AudioFrame<1> &frame : collector.frames)
    rms += frame[0] * frame[0];
  rms /= collector.frames.size();
  rms = std::sqrt(rms);

  EXPECT_LE(Clover::Util::Calc::ltodb(rms), -60);
}

TEST(NodeSimplex_Filter_Filter, ShouldFilterHighPass) {

  int testSize = 10000;
  Clover::_Test::HandCrank crank;
  Clover::_Test::Collector<1> collector(testSize);
  Clover::NodeSimplex::Filter::Filter<1> filter;
  Clover::NodeSimplex::Wavetable::WavetableOsc osc;

  osc >> filter >> collector >> crank;

  osc.sine();
  osc.freq(20);

  filter.highPass();
  filter.set(16000, 10);

  crank.turn(testSize);

  float rms = 0;
  for (Clover::Graph::AudioFrame<1> &frame : collector.frames)
    rms += frame[0] * frame[0];
  rms /= collector.frames.size();
  rms = std::sqrt(rms);

  EXPECT_LE(Clover::Util::Calc::ltodb(rms), -53);
}