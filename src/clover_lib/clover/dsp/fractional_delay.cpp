// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <ranges>

#include "clover/dsp/fractional_delay.hpp"
#include "clover/dsp/interpolate_lagrange.hpp"
#include "clover/dsp/interpolate_sinc.hpp"
#include "clover/float.hpp"

namespace clover::dsp {

fdl_lagrange::fdl_lagrange(size_t buffer_length)
    : m_buffer(buffer_length), m_max_idx(clover_float(buffer_length - 3)) {
}

clover_float fdl_lagrange::at(clover_float idx) {
    if (idx < 2)
        idx = 0;
    else if (idx > m_max_idx)
        idx = m_max_idx;

    auto p2   = static_cast<size_t>(std::floor(idx));
    size_t p1 = p2 - 1;
    size_t p3 = p1 + 1;
    size_t p4 = p2 + 2;

    clover_float interpolation = idx - std::floor(idx);

    return interpolate_lagrange(
            m_buffer[p1],  //
            m_buffer[p2],
            m_buffer[p3],
            m_buffer[p4],
            interpolation);
}

void fdl_lagrange::tick(clover_float x) {
    m_buffer.tick(x);
}

fdl_lagrange_2::fdl_lagrange_2(size_t buffer_length)
    : m_buffer(buffer_length), m_max_idx(clover_float(buffer_length) - 3) {
}

std::pair<clover_float, clover_float> fdl_lagrange_2::at(clover_float idx) {
    if (idx < 2)
        idx = 0;
    else if (idx > m_max_idx)
        idx = m_max_idx;

    auto p2   = static_cast<size_t>(std::floor(idx));
    size_t p1 = p2 - 1;
    size_t p3 = p1 + 1;
    size_t p4 = p2 + 2;

    clover_float interpolation = idx - std::floor(idx);

    auto [p1_L, p1_R] = m_buffer[p1];
    auto [p2_L, p2_R] = m_buffer[p2];
    auto [p3_L, p3_R] = m_buffer[p3];
    auto [p4_L, p4_R] = m_buffer[p4];

    return {
            interpolate_lagrange(p1_L, p2_L, p3_L, p4_L, interpolation),
            interpolate_lagrange(p1_R, p2_R, p3_R, p4_R, interpolation),
    };
}

void fdl_lagrange_2::tick(clover_float x1, clover_float x2) {
    m_buffer.tick(x1, x2);
}

fdl_sinc::fdl_sinc(std::shared_ptr<circular_buffer>& buffer, size_t kernel_size, clover_float window_alpha)
    : m_buffer(buffer), m_kernel(kernel_size, window_alpha, 0) {
    m_min_idx = std::floor(static_cast<clover_float>(kernel_size) / 2) - 1;
    m_max_idx = static_cast<clover_float>(buffer->length()) - 1 -
                std::ceil(static_cast<clover_float>(kernel_size) / 2);
    m_start_index = 0;
}

void fdl_sinc::delay(clover_float idx) {
    if (idx > m_max_idx) {
        idx = m_max_idx;
    } else if (idx < m_min_idx) {
        idx = 0;
    }

    m_delay       = idx;
    m_start_index = static_cast<size_t>(m_delay - m_min_idx);
    m_kernel.set_interpolation(idx);
}

clover_float fdl_sinc::calculate() {
    if (m_delay == 0) {
        return (*m_buffer)[0];
    }

    clover_float dot_product = 0;
    for (auto i : std::views::iota(0, static_cast<int>(m_kernel.size()))) {
        dot_product += m_kernel[i] * (*m_buffer)[m_start_index + i];
    }

    return dot_product;
}

}  // namespace clover::dsp
