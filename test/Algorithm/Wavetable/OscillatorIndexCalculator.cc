
#include <gtest/gtest.h>

#include <vector>

#include "Algo/Wavetable/OscillatorIndexCalculator.h"

TEST(Algorithm_Wavetable_Oscillator_Index_Calculator, ShouldInitialize) {
  float sampleRate = 48000.f;
  int size = 100;
  Clover::Wavetable::OscillatorIndexCalculator<float> calculator(sampleRate,
                                                                 size);
  ASSERT_FLOAT_EQ(calculator.sampleRate(), sampleRate);
  ASSERT_FLOAT_EQ(calculator.size(), size);

  ASSERT_FLOAT_EQ(calculator.freq(), 0.f);
  ASSERT_FLOAT_EQ(calculator.period(), 0.f);
  ASSERT_FLOAT_EQ(calculator.phase(), 0.f);
  ASSERT_FLOAT_EQ(calculator.phaseOffset(), 0.f);

  ASSERT_FLOAT_EQ(calculator.last().indexA, 0.f);
  ASSERT_FLOAT_EQ(calculator.last().indexB, 0.f);
  ASSERT_FLOAT_EQ(calculator.last().lerpAmount, 0.f);

  calculator.process();
  ASSERT_FLOAT_EQ(calculator.last().indexA, 0.f);
  ASSERT_FLOAT_EQ(calculator.last().indexB, 1.f);
  ASSERT_FLOAT_EQ(calculator.last().lerpAmount, 0.f);
  calculator.process();
  ASSERT_FLOAT_EQ(calculator.last().indexA, 0.f);
  ASSERT_FLOAT_EQ(calculator.last().indexB, 1.f);
  ASSERT_FLOAT_EQ(calculator.last().lerpAmount, 0.f);
}

TEST(Algorithm_Wavetable_Oscillator_Index_Calculator, ShouldBeSettable) {
  float sampleRate = 48000.f;
  int size = 100;
  Clover::Wavetable::OscillatorIndexCalculator<float> calculator(sampleRate,
                                                                 size);

  float freq = 1000.f;
  calculator.freq(freq);
  ASSERT_FLOAT_EQ(calculator.freq(), freq);

  float periodFreq = 2.f;
  float period = sampleRate / periodFreq;
  calculator.period(period);
  ASSERT_FLOAT_EQ(calculator.period(), period);
  ASSERT_FLOAT_EQ(calculator.freq(), periodFreq);

  float phase = 0.5f;
  calculator.phase(phase);
  ASSERT_FLOAT_EQ(calculator.phase(), phase);

  float phaseOffset = 0.25;
  calculator.phaseOffset(phaseOffset);
  ASSERT_FLOAT_EQ(calculator.phaseOffset(), phaseOffset);
  ASSERT_FLOAT_EQ(calculator.phase(), phase);
}

TEST(Algorithm_Wavetable_Oscillator_Index_Calculator, ShouldWalkThroughIndex) {
  float sampleRate = 100.f;
  int size = 100;
  Clover::Wavetable::OscillatorIndexCalculator<float> calculator(sampleRate,
                                                                 size);
  calculator.freq(1.f);

  calculator.process();
  ASSERT_FLOAT_EQ(calculator.last().indexA, 0.f);
  ASSERT_FLOAT_EQ(calculator.last().indexB, 1.f);
  ASSERT_FLOAT_EQ(calculator.last().lerpAmount, 0.f);
  calculator.process();
  ASSERT_FLOAT_EQ(calculator.last().indexA, 1.f);
  ASSERT_FLOAT_EQ(calculator.last().indexB, 2.f);
  ASSERT_FLOAT_EQ(calculator.last().lerpAmount, 0.f);
  calculator.process();
  ASSERT_FLOAT_EQ(calculator.last().indexA, 2.f);
  ASSERT_FLOAT_EQ(calculator.last().indexB, 3.f);
  ASSERT_FLOAT_EQ(calculator.last().lerpAmount, 0.f);

  calculator.freq(2.f);
  calculator.process();
  ASSERT_FLOAT_EQ(calculator.last().indexA, 3.f);
  ASSERT_FLOAT_EQ(calculator.last().indexB, 4.f);
  ASSERT_FLOAT_EQ(calculator.last().lerpAmount, 0.f);
}

TEST(Algorithm_Wavetable_Oscillator_Index_Calculator,
     PhaseControlShouldSetReadIndex) {
  float sampleRate = 100.f;
  int size = 100;
  Clover::Wavetable::OscillatorIndexCalculator<float> calculator(sampleRate,
                                                                 size);
  calculator.freq(1.f);

  // immediately changes index
  calculator.phase(0.25f);
  ASSERT_FLOAT_EQ(calculator.process().indexA, 25.f);
  ASSERT_FLOAT_EQ(calculator.process().indexA, 26.f);

  // phase percent wraps
  calculator.phase(1.25f);
  ASSERT_FLOAT_EQ(calculator.process().indexA, 25.f);

  calculator.phase(-0.25f);
  ASSERT_FLOAT_EQ(calculator.process().indexA, 75.f);
}

TEST(Algorithm_Wavetable_Oscillator_Index_Calculator,
     PhaseOffsetShouldOffsetIndex) {
  float sampleRate = 100.f;
  int size = 100;
  Clover::Wavetable::OscillatorIndexCalculator<float> calculator(sampleRate,
                                                                 size);
  calculator.phaseOffset(0.25f);
  calculator.freq(1.f);
  ASSERT_FLOAT_EQ(calculator.phase(), 0.f);
  ASSERT_FLOAT_EQ(calculator.process().indexA, 25.f);

  calculator.phase(0.25f);
  ASSERT_FLOAT_EQ(calculator.process().indexA, 50.f);
  ASSERT_FLOAT_EQ(calculator.process().indexA, 51.f);
}
