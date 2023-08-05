
#include <gtest/gtest.h>

#include <vector>

#include "Algo/Wavetable/OscillatorInterface.h"
#include "Algo/Wavetable/WavetableOscillatorMono.h"

TEST(Algorithm_Wavetable_Oscillator_Mono, ShouldOutputSine) {
  float sampleRate = 48000.f;
  int size = 512;
  Clover::Wavetable::WavetableOscillatorMono<float> osc(sampleRate);

  osc.freq(100);

  int numSamples = sampleRate * 2;
  float accumulator = 0.f;
  for (int i = 0; i < numSamples; i++) {
    float val = osc.process();
    accumulator += (val * val);
  }
  float RMS = sqrt(accumulator / static_cast<float>(numSamples));
  float expectedRms = 1.f / sqrtf(2.f);

  ASSERT_NEAR(RMS, expectedRms, 0.000107f);
}
