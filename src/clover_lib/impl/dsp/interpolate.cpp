// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <ranges>
#include <vector>

#include "clover/circular_buffer.hpp"
#include "clover/dsp/interpolate.hpp"
#include "clover/float.hpp"
#include "clover/num.hpp"

namespace clover::dsp {

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

clover_float interpolate(circular_buffer buffer, std::vector<clover_float>& kernel) {
    int kernel_size = static_cast<int>(kernel.size());

    clover_float dot_product = 0;
    for (auto i : std::views::iota(0, kernel_size - 1))
        dot_product += kernel[i] * buffer[i];

    return dot_product;
}

// TODO: short circuit in simple cases:
// - when interpolation_amount is exactly 0 or 1 and size is even:
//   - all values are 0 except
//   - when interpolation_amount = 0: left of center is 1
//   - when interpolation_amount = 1: right of center is 1
// - when interpolation_amount is exactly 0 and size is odd:
//   - all values are 0 except center is 1
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
