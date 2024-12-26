#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <ranges>
#include <vector>

#include "clover/float.hpp"
#include "clover/num.hpp"

namespace clover::dsp {

std::vector<clover_float> generalized_hann_window(int size, clover_float alpha) {
    std::vector<clover_float> window;
    window.resize(size, 0);

    constexpr clover_float half = 0.5;
    for (auto n : std::views::iota(0, (size - 1) / 2)) {
        clover_float window_value =
                alpha + (1 - alpha) * half *
                                (1 - std::cos((2 * num::pi * (clover_float)n) / ((clover_float)size - 1)));


        window[n]            = window_value;
        window[size - 1 - n] = window_value;
        
        window.emplace_back(window_value);
    }

    return window;
}

std::vector<clover_float> sinc(int size, clover_float interpolate) {
    std::vector<clover_float> sinc;
    sinc.resize(size, 0);

    for (auto x : std::views::iota(0, (size - 1) / 2)) {
        clover_float sinc_val = 1;
        if (x != 0) {
            clover_float pi_x = num::pi * (float)x;
            sinc_val          = std::sin(pi_x) / pi_x;
        }

        sinc[x]            = sinc_val;
        sinc[size - 1 - x] = sinc_val;
    }

    return sinc;
}

std::vector<clover_float> kernel(std::vector<clover_float>& window, std::vector<clover_float>& sinc_coefficients) {
    std::vector<clover_float> kernel;
    kernel.reserve(window.size());

    for (auto i : std::views::iota(0, (int) window.size() - 1)) {
        kernel.emplace_back(window[i] * sinc_coefficients[i]);
    }

    return kernel;
}

std::vector<clover_float> windowed_sinc(std::vector<clover_float>& result, std::vector<clover_float>& signal, std::vector<clover_float>& kernel) {
    for (auto i : std::views::iota(0, (int) signal.size() - 1)) {
        result[i] = signal[i] * kernel[i];
    }

    return kernel;
}

struct interpolate {};

}  // namespace clover::dsp