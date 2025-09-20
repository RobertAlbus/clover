#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <vector>

#include "clover/circular_buffer.hpp"

namespace clover::dsp {

struct sinc_kernel {
    std::vector<float> m_sinc_kernel;
    std::vector<float> m_window;

    sinc_kernel(size_t size, float hann_alpha, float interpolation_amount = 0);
    void set_interpolation(float interpolation_amount);
    float& operator[](size_t idx);
    size_t size();
};

// overwrites a with a*b
void hadamard_product(std::vector<float>& a, std::vector<float>& b);

void hann_window_with_corner_control(std::vector<float>& out, float alpha);

/*
- returns the dot product of a signal with a kernel
- does not provide bounds checking
- start_idx is the position in the buffer that aligns with kernel[0]
*/
float interpolate_sinc(circular_buffer buffer, std::vector<float>& kernel, size_t start_idx);

void sinc_function(std::vector<float>& out, float interpolation_amount);

}  // namespace clover::dsp
