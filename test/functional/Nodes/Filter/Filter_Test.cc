/*
 * /////////
 * // Clover
 *
 * Audio processing algorithms and DAG with feedback loops that do not break
 * acyclicity.
 *
 * Copyright (C) 2023 Rob W. Albus
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version. This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <gtest/gtest.h>

#include <cmath>
#include <limits>

#include "Clover/Nodes/Filter/Filter.h"
#include "Clover/Nodes/Wavetable/WavetableOsc.h"
#include "Clover/_Test.h"

TEST(Nodes_Filter_Filter, ShouldFilterLowPass) {

  int testSize = 10000;
  Clover::_Test::HandCrank crank;
  Clover::_Test::Collector<1> collector(testSize);
  Clover::Nodes::Filter::Filter<1> filter;
  Clover::Nodes::Wavetable::WavetableOsc osc;

  osc >> filter >> collector >> crank;

  osc.sine();
  osc.freq(20000);

  filter.lowPass();
  filter.set(100, 10);

  crank.turn(testSize);

  float rms = 0;
  for (Clover::Graph::AudioFrame<1> &frame : collector.frames)
    rms += frame[0] * frame[0];
  rms /= static_cast<float>(collector.frames.size());
  rms = std::sqrt(rms);

  EXPECT_GT(static_cast<float>(collector.frames.size()), 0.f);
  EXPECT_LE(Clover::Algorithm::linear_to_db(rms), -60);
}

TEST(Nodes_Filter_Filter, ShouldFilterHighPass) {

  size_t testSize = 10000;
  Clover::_Test::HandCrank crank;
  Clover::_Test::Collector<1> collector(testSize);
  Clover::Nodes::Filter::Filter<1> filter;
  Clover::Nodes::Wavetable::WavetableOsc osc;

  osc >> filter >> collector >> crank;

  osc.sine();
  osc.freq(20);

  filter.highPass();
  filter.set(16000, 10);

  crank.turn(testSize);

  float rms = 0;
  for (Clover::Graph::AudioFrame<1> &frame : collector.frames) {
    rms += frame[0] * frame[0];
  }
  rms /= static_cast<float>(collector.frames.size());
  rms = std::sqrt(rms);

  EXPECT_GT(static_cast<float>(collector.frames.size()), 0.f);
  EXPECT_LE(Clover::Algorithm::linear_to_db(rms), -53);
}
