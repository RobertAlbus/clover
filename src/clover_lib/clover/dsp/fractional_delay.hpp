#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/circular_buffer.hpp"
#include "clover/dsp/interpolate_sinc.hpp"
#include "clover/float.hpp"

namespace clover::dsp {

struct fdl_lagrange {
    clover_float m_max_idx;
    circular_buffer& m_buffer;

    fdl_lagrange(circular_buffer& buffer);
    clover_float at(clover_float idx);
};

struct fdl_sinc {
    sinc_kernel m_kernel;
    circular_buffer& m_buffer;
    clover_float m_max_idx;
    clover_float m_min_idx;
    clover_float m_delay;
    size_t m_start_index;

    fdl_sinc(circular_buffer& buffer, size_t kernel_size, clover_float window_alpha = 0.5);

    void delay(clover_float idx);
    clover_float calculate();
};

}  // namespace clover::dsp
