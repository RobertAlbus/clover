// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/math.hpp"

auto clover::frequency_by_octave_difference(clover_float freq, clover_float octaves) -> clover_float {
    return freq * std::exp2(octaves);
}

auto clover::frequency_by_semitone_difference(clover_float freq, clover_float semitones) -> clover_float {
    return frequency_by_octave_difference(freq, semitones * (1.f / 12.f));
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto clover::tension(clover_float value, clover_float tension) -> clover_float {
    clover_float tensionExponent = std::exp2(-tension);
    if (value < 0)
        tensionExponent *= -1;
    return std::pow(std::fabs(value), tensionExponent);
}

auto clover::linear_to_db(clover_float x) -> clover_float {
    return x != 0.f ? std::log10(x) * 20.f : 0.f;
}

auto clover::db_to_linear(clover_float x) -> clover_float {
    return std::pow(10.f, x * 0.05f);
}

auto clover::db_gain_map(clover_float x) -> clover_float {
    return std::log10(1.f + (x * 99.f)) * 0.5f;
}

auto db_gain_map_inverse(clover_float y) -> clover_float {
    return (std::pow(10.0f, 2.0f * y) - 1) * (1.f / 99.f);
}

auto clover::midi_to_frequency(clover_float x) -> clover_float {
    return 440.f * std::exp2((x - 69.f) * (1.f / 12.f));
}

auto clover::frequency_to_midi(clover_float x) -> clover_float {
    return 12.f * (std::log(x * (1.f / 220.f)) / std::log(2.f)) + 57.f;
}
