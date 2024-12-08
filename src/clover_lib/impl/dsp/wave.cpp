// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "clover/dsp/wave.hpp"
#include "clover/float.hpp"

namespace clover::dsp {

auto wave_sine(clover_float x) -> clover_float {
    return std::sin(x);
}
auto wave_square(clover_float x) -> clover_float {
    return x < num::pi ? 1 : 0;
}
auto wave_saw(clover_float x) -> clover_float {
    return std::lerp(clover_float{1}, clover_float{-1}, x / num::pi_x2);
}
auto wave_tri(clover_float x) -> clover_float {
    x += num::pi / 2;
    if (x > num::pi_x2)
        x -= num::pi_x2;
    return -2 * std::abs((x / num::pi) - 1) + 1;
}

// the following code is adapted from
// https://www.musicdsp.org/en/latest/Synthesis/216-fast-whitenoise-generator.html
// orgiinally by ed.bew@hcrikdlef.dreg

// NOLINTNEXTLINE(bugprone-integer-division)
constexpr clover_float _wave_noise_fScale = 2 / 4294967295.f;

int _wave_noise_x1 = 0x70f4f854;
// NOLINTNEXTLINE(bugprone-narrowing-conversions)
int _wave_noise_x2 = 0xe1e9f0a7;

auto wave_noise(clover_float x) -> clover_float {
    _wave_noise_x1 ^= _wave_noise_x2;
    clover_float signal = static_cast<clover_float>(_wave_noise_x2) * _wave_noise_fScale;
    _wave_noise_x2 += _wave_noise_x1;

    return signal;
}

}  // namespace clover::dsp
