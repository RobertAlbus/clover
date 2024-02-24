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

#include "Clover/Nodes/Basic/Gain.h"
#include "Clover/_Test/Collector.h"
#include "Clover/_Test/DCN.h"
#include "Clover/_Test/HandCrank.h"

TEST(Nodes_Basic_Gain, ShouldNotModify) {
  Clover::_Test::HandCrank crank;
  Clover::_Test::Collector<1> collector(1);
  Clover::Nodes::Basic::Gain<1> g;
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

TEST(Nodes_Basic_Gain, ShouldModify) {
  Clover::_Test::HandCrank crank;
  Clover::_Test::Collector<1> collector(3);
  Clover::Nodes::Basic::Gain<1> g;
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
