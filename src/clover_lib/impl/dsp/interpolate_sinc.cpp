// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <format>
#include <ranges>
#include <vector>

#include "clover/dsp/interpolate_sinc.hpp"
#include "clover/float.hpp"
#include "clover/num.hpp"

namespace clover::dsp {

sinc_kernel::sinc_kernel(size_t size, clover_float hann_alpha, clover_float interpolation_amount) {
    m_window.resize(size, 0);
    m_sinc_kernel.resize(size, 0);

    hann_window_with_corner_control(m_window, hann_alpha);
    set_interpolation(interpolation_amount);
}

void sinc_kernel::set_interpolation(clover_float interpolation_amount) {
    interpolation_amount = interpolation_amount - std::floor(interpolation_amount);
    sinc_function(m_sinc_kernel, interpolation_amount);
    hadamard_product(m_sinc_kernel, m_window);
}

size_t sinc_kernel::size() {
    return m_sinc_kernel.size();
}

clover_float& sinc_kernel::operator[](size_t idx) {
    if (idx < 0 || idx >= m_sinc_kernel.size())
        throw std::out_of_range(std::format("out of range: sinc_kernel::operator[{}]", idx));

    return m_sinc_kernel[idx];
}

void hadamard_product(std::vector<clover_float>& a, std::vector<clover_float>& b) {
    for (auto i : std::views::iota(0, static_cast<int>(a.size())))
        a[i] = a[i] * b[i];
}

void hann_window_with_corner_control(std::vector<clover_float>& out, clover_float alpha) {
    int size = static_cast<int>(out.size());

    // window is symmetrical. only compute half of it.
    for (auto n : std::views::iota(0, (size / 2))) {
        clover_float window_value =
                alpha + (1 - alpha) * 0.5f *
                                (1 - std::cos((2 * num::pi * (clover_float)n) / ((clover_float)size - 1)));

        out[n]            = window_value;
        out[size - 1 - n] = window_value;
    }

    if (size % 2)
        out[(size / 2)] = 1;
}

// TODO: short circuit in simple cases:
// - when interpolation_amount is exactly 0 and
//   - size is even: all values are 0 except left of center is 1
//   - size is odd: all values are 0 except center is 1
void sinc_function(std::vector<clover_float>& out, clover_float interpolation_amount) {
    int size = static_cast<int>(out.size());

    for (auto i : std::views::iota(0, size)) {
        clover_float x = (static_cast<clover_float>(i) - (static_cast<clover_float>(size) / 2)) +
                         (1 - interpolation_amount);

        if (float_eq(x, 0.f)) {
            out[i] = 1;
            continue;
        }

        clover_float pi_x = num::pi * x;
        out[i]            = (std::sin(pi_x) / pi_x);
    }
}

}  // namespace clover::dsp
