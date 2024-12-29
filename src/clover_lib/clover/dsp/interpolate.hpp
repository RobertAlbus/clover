#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <vector>

#include "clover/circular_buffer.hpp"
#include "clover/float.hpp"

namespace clover::dsp {

// overwrites a with a*b
void hadamard_product(std::vector<clover_float>& a, std::vector<clover_float>& b);

void hann_window_with_corner_control(std::vector<clover_float>& out, clover_float alpha);

// returns the dot product of a signal with a kernel
clover_float interpolate(circular_buffer buffer, std::vector<clover_float>& kernel);

void sinc_function(std::vector<clover_float>& out, clover_float interpolation_amount);

}  // namespace clover::dsp
