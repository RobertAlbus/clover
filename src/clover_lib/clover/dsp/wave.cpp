// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "clover/dsp/wave.hpp"
#include "clover/num.hpp"

namespace clover::dsp {

auto wave_sine(float x) -> float {
    return std::sin(x);
}
auto wave_square(float x) -> float {
    return x < num::pi ? 1 : -1;
}
auto wave_saw(float x) -> float {
    return std::lerp(float{1}, float{-1}, x / num::pi_x2);
}
auto wave_tri(float x) -> float {
    x += num::pi / 2;
    if (x > num::pi_x2)
        x -= num::pi_x2;
    return -2 * std::abs((x / num::pi) - 1) + 1;
}

// the following code is adapted from
// https://www.musicdsp.org/en/latest/Synthesis/216-fast-whitenoise-generator.html
// orgiinally by ed.bew@hcrikdlef.dreg

// NOLINTNEXTLINE(bugprone-integer-division)
constexpr float _wave_noise_fScale = 2 / 4294967295.f;

int _wave_noise_x1 = 0x70f4f854;
// NOLINTNEXTLINE(bugprone-narrowing-conversions)
int _wave_noise_x2 = 0xe1e9f0a7;

auto wave_noise(float x) -> float {
    _wave_noise_x1 ^= _wave_noise_x2;
    float signal = static_cast<float>(_wave_noise_x2) * _wave_noise_fScale;
    _wave_noise_x2 += _wave_noise_x1;

    return signal;
}

}  // namespace clover::dsp
