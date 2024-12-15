// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdio>
#include <gtest/gtest.h>
#include <vector>

#include "clover/float.hpp"
#include "clover/math.hpp"

using namespace clover;

struct decibel_conversions {
    clover_float linear;
    clover_float db;
};
TEST(math, linear_to_db) {
    std::vector<decibel_conversions> fixtures{
            {4, 12},
            {2, 6},
            {1, 0},
            {0.5, -6},
            {0.25, -12},
    };
    for (auto fixture : fixtures) {
        EXPECT_NEAR(linear_to_db(fixture.linear), fixture.db, 0.05);
    }
}

TEST(math, db_to_linear) {
    std::vector<decibel_conversions> fixtures{
            {4, 12},
            {2, 6},
            {1, 0},
            {0.5, -6},
            {0.25, -12},
    };
    for (auto fixture : fixtures) {
        EXPECT_NEAR(db_to_linear(fixture.db), fixture.linear, 0.05);
    }
}

TEST(math, frequency_by_octave_difference) {
    std::vector<clover_float> inputs{1, 2, 10, 20, 200, 200, 1000, 2000, 10000, 20000};
    std::vector<clover_float> octaves{-3, -2, -1, 0, 1, 2, 3};
    for (auto input : inputs) {
        for (auto octave : octaves) {
            EXPECT_EQ(frequency_by_octave_difference(input, octave), input * std::exp2(octave));
        }
    }
}

TEST(math, frequency_by_semitone_difference) {
    std::vector<clover_float> inputs{1, 2, 10, 20, 200, 200, 1000, 2000, 10000, 20000};
    std::vector<clover_float> octaves{-3, -2, -1, 0, 1, 2, 3};
    for (auto input : inputs) {
        for (auto octave : octaves) {
            EXPECT_EQ(frequency_by_semitone_difference(input, octave * 12.f), input * std::exp2(octave));
        }
    }
}

struct midi_freq {
    clover_float midi_note;
    clover_float frequency;
};

TEST(math, frequency_to_midi) {
    std::vector<midi_freq> fixtures{
            {0, 8.175798f},
            {63, 311.126983722f},
            {127, 12543.8555f},
    };
    for (auto fixture : fixtures)
        EXPECT_FLOAT_EQ(frequency_to_midi(fixture.frequency), fixture.midi_note);
}

TEST(math, midi_to_frequency) {
    std::vector<midi_freq> fixtures{
            {0, 8.175798f},
            {63, 311.126983722f},
            {127, 12543.8555f},
    };
    for (auto fixture : fixtures)
        EXPECT_FLOAT_EQ(midi_to_frequency(fixture.midi_note), fixture.frequency);
}

TEST(math, tension) {
    EXPECT_NEAR(tension(0.5f, 1.f), -tension(-0.5f, 1.f), 0.006f);
    EXPECT_NEAR(tension(0.5f, 1.f), 1.f - tension(0.5f, -1.f), 0.006f);
    EXPECT_GT(tension(0.5f, 1.f), tension(0.5f, 0.5f));
    EXPECT_GT(tension(0.6f, 1.f), tension(0.5f, 1.f));
}
