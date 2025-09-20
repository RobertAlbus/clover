#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/circular_buffer.hpp"
#include "clover/dsp/interpolate_sinc.hpp"

namespace clover::dsp {

struct fdl_lagrange {
    float m_max_idx;
    circular_buffer m_buffer;

    fdl_lagrange(size_t buffer_length);
    float at(float idx);
    void tick(float x);
};

struct fdl_lagrange_2 {
    float m_max_idx;
    circular_buffer_2 m_buffer;

    fdl_lagrange_2(size_t buffer_length);
    std::pair<float, float> at(float idx);
    void tick(float x1, float x2);
};

struct fdl_sinc {
    sinc_kernel m_kernel;
    std::shared_ptr<circular_buffer> m_buffer;
    float m_max_idx;
    float m_min_idx;
    float m_delay;
    size_t m_start_index;

    fdl_sinc(std::shared_ptr<circular_buffer>& buffer, size_t kernel_size, float window_alpha = 0.5);

    void delay(float idx);
    float calculate();
};

}  // namespace clover::dsp
