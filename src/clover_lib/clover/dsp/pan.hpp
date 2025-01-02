#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.
#include <cmath>
#include <utility>

#include "clover/float.hpp"

namespace clover::dsp {

struct pan {
    clover_float m_pan_L = std::sqrt(clover_float(2));
    clover_float m_pan_R = std::sqrt(clover_float(2));

    std::pair<clover_float, clover_float> process(clover_float in);
    std::pair<clover_float, clover_float> process(clover_float L, clover_float R);
    void set(clover_float pan);
};

}  // namespace clover::dsp
