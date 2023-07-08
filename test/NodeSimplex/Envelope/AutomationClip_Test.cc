#include <gtest/gtest.h>

#include "Clover.h"

TEST(NodeSimplex_Envelope_AutomationClip, ShouldOutputEnvelope) {

  Clover::_Test::HandCrank<1> crank;
  Clover::_Test::Collector<1> collector(1000);
  Clover::NodeSimplex::AutomationNode automationClip;

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
  EXPECT_EQ(collector.frames[250][0], 1.5f);
  EXPECT_EQ(collector.frames[300][0], 0.5f);
}

TEST(NodeSimplex_Envelope_AutomationClip, ShouldHaveDefaultEnvelope) {

  Clover::_Test::HandCrank<1> crank;
  Clover::_Test::Collector<1> collector(10);
  Clover::NodeSimplex::AutomationNode automationClip;

  automationClip >> collector >> crank;

  EnvelopeDefinition envDef;

  crank.turn(10);

  EXPECT_EQ(collector.frames[0][0], 0.f);
  EXPECT_EQ(collector.frames[9][0], 0.f);
}

TEST(NodeSimplex_Envelope_AutomationClip, ShouldHoldLastValue) {

  Clover::_Test::HandCrank<1> crank;
  Clover::_Test::Collector<1> collector(10);
  Clover::NodeSimplex::AutomationNode automationClip;

  automationClip >> collector >> crank;

  EnvelopeDefinition envDef;
  envDef.addPoint(0, 0.0);
  envDef.addPoint(100, 0.5);

  automationClip.useEnvelope(envDef);

  crank.turn(201);

  EXPECT_EQ(collector.frames[0][0], 0.f);
  EXPECT_EQ(collector.frames[200][0], 0.5f);
}
