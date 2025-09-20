#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.
#include <cmath>
#include <utility>


namespace clover::dsp {

struct pan {
    float m_pan_L = std::sqrt(float(2));
    float m_pan_R = std::sqrt(float(2));

    std::pair<float, float> process(float in);
    std::pair<float, float> process(float L, float R);
    void set(float pan);
};

}  // namespace clover::dsp
