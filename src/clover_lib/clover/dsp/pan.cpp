// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/dsp/pan.hpp"
#include "clover/num.hpp"

namespace clover::dsp {

std::pair<float, float> pan::process(float in) {
    return {m_pan_L * in, m_pan_R * in};
}

std::pair<float, float> pan::process(float L, float R) {
    return {m_pan_L * L, m_pan_R * R};
}

void pan::set(float pan) {
    m_pan_L = std::cos(0.25f * num::pi * (pan + 1));
    m_pan_R = std::sin(0.25f * num::pi * (pan + 1));
}

}  // namespace clover::dsp
