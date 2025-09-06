// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdint>

#include "clover/float.hpp"
#include "clover/math.hpp"

auto clover::db_curve(clover_float x, clover_float n) -> clover_float {
    return std::max(0.f, 0.5f * std::log(x / n) + n);
}

auto clover::db_curve_inverse(clover_float x, clover_float n) -> clover_float {
    return n - clover::db_curve(n - x, n);
}

auto clover::db_to_linear(clover_float x) -> clover_float {
    return std::pow(10.f, x * 0.05f);
}

auto clover::linear_to_db(clover_float x) -> clover_float {
    return 20.0f * std::log10(std::max(x, 1e-6f));
}

auto clover::frequency_by_octave_difference(clover_float freq, clover_float octaves) -> clover_float {
    return freq * std::exp2(octaves);
}

auto clover::frequency_by_semitone_difference(clover_float freq, clover_float semitones) -> clover_float {
    return frequency_by_octave_difference(freq, semitones * (1.f / 12.f));
}

auto clover::octave_difference_by_frequency(clover_float base_freq, clover_float target_freq)
        -> clover_float {
    return std::log2(target_freq / base_freq);
}

auto clover::semitone_difference_by_frequency(clover_float base_freq, clover_float target_freq)
        -> clover_float {
    return octave_difference_by_frequency(base_freq, target_freq) * 12.f;
}

auto clover::frequency_to_midi(clover_float x) -> clover_float {
    if (x <= 0)
        return 0;
    return 12.f * (std::log(x * (1.f / 220.f)) / std::log(2.f)) + 57.f;
}

auto clover::midi_to_frequency(clover_float x) -> clover_float {
    return 440.f * std::exp2((x - 69.f) * (1.f / 12.f));
}

auto clover::sign(clover_float x) -> clover_float {
    return x >= 1 ? 1 : -1;
}

auto clover::tension(clover_float x, clover_float a) -> clover_float {
    if (float_eq(a, 0))
        return x;

    a *= 24.f;

    uint8_t flags = 0 | (x < 0) | (a < 0) << 1;

    static constexpr uint8_t POS_POS = 0;
    static constexpr uint8_t NEG_POS = 1;
    static constexpr uint8_t POS_NEG = 2;
    static constexpr uint8_t NEG_NEG = 3;

    switch (flags) {
        case POS_POS:
            // std::printf("\npos pos - %f %f", x, a);
            return tension_pos_a(x, a);
        case NEG_POS:
            // std::printf("\nneg pos - %f %f", x, a);
            return tension_neg_a(1.f + x, a) - 1.f;
        case POS_NEG:
            // std::printf("\npos neg - %f %f", x, a);
            return tension_neg_a(x, -a);
        case NEG_NEG:
            // std::printf("\nneg neg - %f %f", x, a);
            return tension_pos_a(1.f + x, -a) - 1.f;
        default:
            return 0.f;
    }
}

auto clover::tension_pos_a(clover_float x, clover_float a) -> clover_float {
    return 1.f - ((1.f - x) / (1.f + (40 * a * x)));
}

auto clover::tension_neg_a(clover_float x, clover_float a) -> clover_float {
    return (x / (1.f + (40 * -a * (1.f - x))));
}
