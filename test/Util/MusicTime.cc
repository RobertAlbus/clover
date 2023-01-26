#include <gtest/gtest.h>

#include "Clover.h"

TEST(Util_MusicTime, ShouldProvideCorrectTime) {
  float sampleRate = 48000.0;
  float minute = 60. * sampleRate;
  float bpm = 159.;

  float beatLength = minute / bpm;

  SampleClock clock;
  Time time(bpm, (int)sampleRate, &clock);

  int duration_two_and_a_half_beats = (int)(beatLength * 2.5);
  for (int i = 0; i < duration_two_and_a_half_beats; i++) {
    clock.tick();
  }

  EXPECT_EQ(time.beat(), beatLength);
  EXPECT_EQ((float)time.beat_i(), (int)beatLength);
  EXPECT_EQ(time.currentBeat(),
            ((float)duration_two_and_a_half_beats / time.beat()));
  EXPECT_EQ(time.currentBeat_i(), 2);
}