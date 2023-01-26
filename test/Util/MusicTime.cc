#include <gtest/gtest.h>

#include "Clover.h"

TEST(Util_MusicTime, ShouldProvideCorrectTime) {
    float sampleRate = 48000.0;
    float minute = 60. * sampleRate;
    float bpm = 160.;
    
    float beatLength = minute / bpm;
    
    SampleClock clock;
    Time time(bpm, (int)sampleRate, &clock);

    int two_and_a_half = (int) (beatLength * 2.5);
    for (int i = 0; i < two_and_a_half; i++) {
        clock.tick();
    }

    EXPECT_EQ(time.beat, beatLength);
    EXPECT_EQ(time.currentBeat(), 2.5);
    EXPECT_EQ(time.currentBeat_i(), 2);
}