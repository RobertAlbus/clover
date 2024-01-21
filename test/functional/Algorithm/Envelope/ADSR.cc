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

#include "Algorithm/Envelope/ADSR.h"

TEST(Algorithm_Envelope_ADSR, ShouldInitializeAsInstanlyFullyOpen) {
  Clover::Envelope::ADSR<float> envelope;

  EXPECT_EQ(envelope.process(), 0.f);
  EXPECT_EQ(envelope.process(), 0.f);

  envelope.keyOn();
  EXPECT_EQ(envelope.process(), 0.f);
  EXPECT_EQ(envelope.process(), 1.f);
  EXPECT_EQ(envelope.process(), 1.f);
  EXPECT_EQ(envelope.process(), 1.f);

  envelope.keyOff();

  EXPECT_EQ(envelope.process(), 0.f);
}

TEST(Algorithm_Envelope_ADSR, ShouldBeSettableAndGettable) {
  Clover::Envelope::ADSR<float> envelope(5, 3, 0.5f, 3);

  EXPECT_EQ(envelope.attack(), 5);
  EXPECT_EQ(envelope.decay(), 3);
  EXPECT_EQ(envelope.sustain(), 0.5f);
  EXPECT_EQ(envelope.release(), 3);

  envelope.attack(10);
  envelope.decay(10);
  envelope.sustain(1.f);
  envelope.release(10);

  EXPECT_EQ(envelope.attack(), 10);
  EXPECT_EQ(envelope.decay(), 10);
  EXPECT_EQ(envelope.sustain(), 1.f);
  EXPECT_EQ(envelope.release(), 10);
}

TEST(Algorithm_Envelope_ADSR, ShouldOutputEnvelope) {
  Clover::Envelope::ADSR<float> envelope(5, 3, 0.5f, 3);

  envelope.keyOn();

  EXPECT_EQ(envelope.process(), 0.f);
  EXPECT_EQ(envelope.process(), 0.25f);
  EXPECT_EQ(envelope.process(), 0.5f);
  EXPECT_EQ(envelope.process(), 0.75f);
  EXPECT_EQ(envelope.process(), 1.f);
  EXPECT_EQ(envelope.process(), 0.75f);
  EXPECT_EQ(envelope.process(), 0.5f);

  envelope.keyOff();

  EXPECT_EQ(envelope.process(), 0.25f);
  EXPECT_EQ(envelope.process(), 0.f);
}

TEST(Algorithm_Envelope_ADSR, ShouldHoldSustainAndRelease) {
  Clover::Envelope::ADSR<float> envelope(5, 3, 0.5f, 3);

  envelope.keyOn();

  // attack and decay overlap by 1 sample
  // so the combined duration = (attack + decay - 1)
  int attackDecayDuration = envelope.attack() + envelope.decay() - 1;
  for (int i = 0; i < attackDecayDuration; i++) {
    envelope.process();
  }

  const int testIterations = 100;
  for (int i = 0; i < testIterations; i++) {
    EXPECT_EQ(envelope.process(), 0.5f)
        << "Expected to hold value. iteration=[" << i << "]";
  }

  envelope.keyOff();

  // sustain release overlap by 1 sample
  // so the duration = (release - 1)
  int releaseDuration = envelope.release() - 1;
  for (int i = 0; i < releaseDuration; i++) {
    envelope.process();
  }

  for (int i = 0; i < testIterations; i++) {
    EXPECT_EQ(envelope.process(), 0.f)
        << "Expected to hold value. iteration=[" << i << "]";
  }
}

TEST(Algorithm_Envelope_ADSR, ShouldBeRetriggerableMidEnvelope) {
  Clover::Envelope::ADSR<float> envelope(5, 3, 0.5f, 3);

  envelope.keyOn();

  EXPECT_EQ(envelope.process(), 0.f);
  EXPECT_EQ(envelope.process(), 0.25f);
  EXPECT_EQ(envelope.process(), 0.5f);
  EXPECT_EQ(envelope.process(), 0.75f);
  EXPECT_EQ(envelope.process(), 1.f);
  EXPECT_EQ(envelope.process(), 0.75f);
  EXPECT_EQ(envelope.process(), 0.5f);

  envelope.keyOn();

  EXPECT_EQ(envelope.process(), 0.f);
  EXPECT_EQ(envelope.process(), 0.25f);
  EXPECT_EQ(envelope.process(), 0.5f);
  EXPECT_EQ(envelope.process(), 0.75f);
  EXPECT_EQ(envelope.process(), 1.f);
  EXPECT_EQ(envelope.process(), 0.75f);
  EXPECT_EQ(envelope.process(), 0.5f);

  envelope.keyOff();

  EXPECT_EQ(envelope.process(), 0.25f);
  EXPECT_EQ(envelope.process(), 0.f);
}

TEST(Algorithm_Envelope_ADSR, ShouldBeAbleToUpdateAttackDuringAttack) {
  Clover::Envelope::ADSR<float> envelope(101, 3, 0.5f, 3);
  /*
  - 101 sample attack, 100 increment periods
  - halfway mark: 50 samples, value of 0.5f
  
  */

  envelope.keyOn();

  float result;
  int halfAttackDuration = 50;
  for (int i = 0; i <= halfAttackDuration; i++) {
    result = envelope.process();
    EXPECT_EQ(result, static_cast<float>(i) * 0.01f);
  }

  EXPECT_EQ(result, 0.5f);

  // 11 remaining samples in the new attack, which is 10 increments
  // 0.5 remaining distance divided by 10 is 0.05
  // remaining attack should increase by increments of 0.05
  envelope.attack(61);

  EXPECT_EQ(envelope.process(), 0.55f);
  EXPECT_EQ(envelope.process(), 0.6f);
  EXPECT_EQ(envelope.process(), 0.65f);
  EXPECT_EQ(envelope.process(), 0.7f);
  EXPECT_EQ(envelope.process(), 0.75f);
  EXPECT_EQ(envelope.process(), 0.8f);
  EXPECT_EQ(envelope.process(), 0.85f);
  EXPECT_EQ(envelope.process(), 0.9f);
  EXPECT_NEAR(
      envelope.process(),
      0.95f,
      0.000000065
  ); // sorry, floats are weird
  EXPECT_EQ(envelope.process(), 1.f);
}

TEST(Algorithm_Envelope_ADSR, ShouldBeAbleToUpdateDecayDuringDecay) {
  Clover::Envelope::ADSR<float> envelope(3, 101, 0.5f, 3);

  envelope.keyOn();

  float result;
  int flush = envelope.attack() + (envelope.decay() / 2);
  for (int i = 0; i < flush; i++) {
    result = envelope.process();
  }

  EXPECT_EQ(result, 0.75f);

  // 11 samples in the new decay, which is 10 increments
  // 0.25 remaining distance divided by 10 is 0.025
  // remaining decay should decrease by increments of 0.025
  envelope.decay(60);

  EXPECT_EQ(envelope.process(), 0.725f);
  EXPECT_EQ(envelope.process(), 0.7f);
  EXPECT_EQ(envelope.process(), 0.675f);
  EXPECT_EQ(envelope.process(), 0.65f);
  EXPECT_EQ(envelope.process(), 0.625f);
  EXPECT_EQ(envelope.process(), 0.6f);
  EXPECT_EQ(envelope.process(), 0.575f);
  EXPECT_EQ(envelope.process(), 0.55f);
  EXPECT_EQ(envelope.process(), 0.525f);
  EXPECT_EQ(envelope.process(), 0.5f);
}

TEST(Algorithm_Envelope_ADSR, ShouldBeAbleToUpdateSustainDuringDecay) {
  Clover::Envelope::ADSR<float> envelope(3, 11, 0.5f, 3);

  envelope.keyOn();

  float result;
  int flush = envelope.attack() + (envelope.decay() / 2);
  for (int i = 0; i < flush; i++) {
    result = envelope.process();
  }

  EXPECT_EQ(result, 0.75f);

  // 6 remaining samples in decay, which is 50 increments
  // 0.75 remaining distance divided by 6 is 0.15
  // remaining decay should decrease by increments of 0.15
  envelope.sustain(0.f);

  flush = (envelope.decay() / 2) + 1;
  for (int i = 0; i < flush; i++) {
    float expectedValue = 0.75f - (static_cast<float>(i + 1) * 0.125f);
    EXPECT_NEAR(
        envelope.process(),
        expectedValue,
        0.0001
    ); // sorry, floats are weird
  }
}

TEST(Algorithm_Envelope_ADSR, ShouldBeAbleToUpdateSustainDuringSustain) {
  Clover::Envelope::ADSR<float> envelope(3, 2, 0.5f, 3);

  envelope.keyOn();

  float result;
  int flush = envelope.attack() + envelope.decay();
  for (int i = 0; i < flush; i++) {
    result = envelope.process();
  }

  EXPECT_EQ(result, 0.5f);

  envelope.sustain(0.6);

  EXPECT_EQ(envelope.process(), 0.6f);
}

TEST(Algorithm_Envelope_ADSR, ShouldBeAbleToUpdateReleaseDuringRelease) {
  Clover::Envelope::ADSR<float> envelope(3, 2, 1.f, 11);

  envelope.keyOn();

  float result;
  int flush = envelope.attack() + envelope.decay() + 10;
  for (int i = 0; i < flush; i++) {
    result = envelope.process();
  }

  EXPECT_EQ(result, 1.f);

  envelope.keyOff();

  flush = envelope.release() / 2;
  for (int i = 0; i < flush; i++) {
    result = envelope.process();
  }

  EXPECT_EQ(result, 0.5f);

  envelope.release(15);

  flush = 10;
  for (int i = 0; i < flush; i++) {
    float expectedValue = 0.5f - (0.05f * static_cast<float>(i + 1));
    EXPECT_EQ(envelope.process(), expectedValue);
  }
}

TEST(Algorithm_Envelope_ADSR, ShouldInvokeCallbackOnCompletion) {
  Clover::Envelope::ADSR<float> envelope(2, 2, 1.f, 2);

  bool hasCompleted = false;
  envelope.onComplete([&hasCompleted](const Clover::Envelope::ADSR<float> *env
                      ) { hasCompleted = true; });

  envelope.keyOn();
  envelope.process();
  envelope.process();
  envelope.process();
  envelope.process();

  envelope.keyOff();
  envelope.process();

  EXPECT_FALSE(hasCompleted);

  envelope.process();

  EXPECT_TRUE(hasCompleted);
}
