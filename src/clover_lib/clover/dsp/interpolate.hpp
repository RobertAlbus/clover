#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <ranges>
#include <stdexcept>
#include <vector>

#include "clover/float.hpp"
#include "clover/num.hpp"

namespace clover::dsp {

namespace {

    bool is_odd(int x) {
        return x % 2;
    }

}  // namespace
// assume even size
void generalized_hann_window(int size, clover_float alpha, clover_float* out) {
    if (is_odd(size))
        throw std::invalid_argument("argument size for generalized_hann_window() must be even");

    clover_float* front = out;
    clover_float* back  = out + size - 1;

    for (auto n : std::views::iota(0, (size - 1) / 2)) {
        clover_float window_value =
                alpha + (1 - alpha) * 0.5f *
                                (1 - std::cos((2 * num::pi * (clover_float)n) / ((clover_float)size - 1)));

        *(front + n) = window_value;
        *(back - n)  = window_value;
    }
}

void sinc(int size, clover_float interpolate, clover_float* out) {
    if (is_odd(size))
        throw std::invalid_argument("argument size for sinc() must be even");

    clover_float* front = out;
    clover_float* back  = out + size - 1;

    for (auto n : std::views::iota(0, (size - 1) / 2)) {
        clover_float sinc_value = 1;
        if (n != 0) {
            clover_float pi_x = num::pi * (float)n;
            sinc_value        = std::sin(pi_x) / pi_x;
        }

        *(front + n) = sinc_value;
        *(back - n)  = sinc_value;
    }
}

void kernel(int size, const clover_float* window, const clover_float* sinc, clover_float* out) {
    if (is_odd(size))
        throw std::invalid_argument("argument size for kernel() must be even");

    for (auto i : std::views::iota(0, size - 1)) {
        *(out + i) = *(window + i) * *(sinc + i);
    }
}

void windowed_sinc(int size, const clover_float* signal, const clover_float* kernel, clover_float* out) {
    if (is_odd(size))
        throw std::invalid_argument("argument size for windowed_sinc() must be even");

    for (auto i : std::views::iota(0, size - 1)) {
        *(out + i) = *(signal + i) * kernel[i];
    }
}

struct interpolate {};

}  // namespace clover::dsp