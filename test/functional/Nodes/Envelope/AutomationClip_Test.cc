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

#include "Clover/Nodes/Envelope/AutomationClip.h"
#include "Clover/Nodes/Envelope/EnvelopeStruct.h"
#include "Clover/_Test.h"

TEST(Nodes_Envelope_AutomationClip, ShouldOutputEnvelope) {

  Clover::_Test::HandCrank crank;
  Clover::_Test::Collector<1> collector(1000);
  Clover::Nodes::AutomationNode automationClip;

  automationClip >> collector >> crank;

  EnvelopeDefinition envDef;
  // clang-format off
  envDef.addPoint(0  , 0.0);
  envDef.addPoint(100, 0.0);
  envDef.addPoint(200, 1.0);
  envDef.addPoint(300, 0.5);
  // clang-format on

  automationClip.useEnvelope(envDef);

  crank.turn(1000);

  EXPECT_EQ(collector.frames[0][0], 0.f);
  EXPECT_EQ(collector.frames[100][0], 0.f);
  EXPECT_EQ(collector.frames[200][0], 1.f);
  EXPECT_EQ(collector.frames[250][0], 0.75f);
  EXPECT_EQ(collector.frames[300][0], 0.5f);
}

TEST(Nodes_Envelope_AutomationClip, ShouldHaveDefaultEnvelope) {

  Clover::_Test::HandCrank crank;
  Clover::_Test::Collector<1> collector(10);
  Clover::Nodes::AutomationNode automationClip;

  automationClip >> collector >> crank;

  EnvelopeDefinition envDef;

  crank.turn(10);

  EXPECT_EQ(collector.frames[0][0], 0.f);
  EXPECT_EQ(collector.frames[9][0], 0.f);
}

TEST(Nodes_Envelope_AutomationClip, ShouldHoldLastValue) {

  Clover::_Test::HandCrank crank;
  Clover::_Test::Collector<1> collector(10);
  Clover::Nodes::AutomationNode automationClip;

  automationClip >> collector >> crank;

  EnvelopeDefinition envDef;
  envDef.addPoint(0, 0.0);
  envDef.addPoint(100, 0.5);

  automationClip.useEnvelope(envDef);

  crank.turn(201);

  EXPECT_EQ(collector.frames[0][0], 0.f);
  EXPECT_EQ(collector.frames[200][0], 0.5f);
}
