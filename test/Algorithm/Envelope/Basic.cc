#include <gtest/gtest.h>

#include "Algo/Envelope/Basic.h"

TEST(Algorithm_Envelope_Basic, ShouldInitializeAtZero) {
  Clover::Envelope::Basic<float> envelope;

  EXPECT_EQ(envelope.last(), 0.f);

  EXPECT_EQ(envelope.process(), 0.f);
  EXPECT_EQ(envelope.process(), 0.f);
}

TEST(Algorithm_Envelope_Basic, CanSetToAndFrom) {
  Clover::Envelope::Basic<float> envelope;

  envelope.set(0.f, 1.f, 5);

  EXPECT_EQ(envelope.process(), 0.0f);
  EXPECT_EQ(envelope.process(), 0.25f);
  EXPECT_EQ(envelope.process(), 0.5f);
  EXPECT_EQ(envelope.process(), 0.75f);
  EXPECT_EQ(envelope.process(), 1.f);

  envelope.set(1.f, 0.f, 5);

  EXPECT_EQ(envelope.process(), 1.f);
  EXPECT_EQ(envelope.process(), 0.75f);
  EXPECT_EQ(envelope.process(), 0.5f);
  EXPECT_EQ(envelope.process(), 0.25f);
  EXPECT_EQ(envelope.process(), 0.0f);
}

TEST(Algorithm_Envelope_Basic, CanSetTo) {
  Clover::Envelope::Basic<float> envelope;

  EXPECT_EQ(envelope.process(), 0.0f);

  envelope.set(1, 5);

  EXPECT_EQ(envelope.process(), 0.25f);
  EXPECT_EQ(envelope.process(), 0.5f);
  EXPECT_EQ(envelope.process(), 0.75f);
  EXPECT_EQ(envelope.process(), 1.f);

  envelope.set(0, 5);

  EXPECT_EQ(envelope.process(), 0.75f);
  EXPECT_EQ(envelope.process(), 0.5f);
  EXPECT_EQ(envelope.process(), 0.25f);
  EXPECT_EQ(envelope.process(), 0.0f);
}

TEST(Algorithm_Envelope_Basic, ShouldHoldTargetValue) {
  uint duration = 5;
  Clover::Envelope::Basic<float> envelope(0.f, 10.005f, 5);

  for (uint i = 0; i < duration * 10u; i++) {
    envelope.process();
  }

  EXPECT_EQ(envelope.last(), 10.005f);

  envelope.set(10.005f, 0.f, 5);

  for (uint i = 0; i < duration * 10u; i++) {
    envelope.process();
  }

  EXPECT_EQ(envelope.last(), 0.f);
}