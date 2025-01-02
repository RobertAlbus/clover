// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/dsp/interpolate_lagrange.hpp"
#include "clover/float.hpp"

namespace clover::dsp {

clover_float interpolate_lagrange(
        clover_float p0, clover_float p1, clover_float p2, clover_float p3, clover_float s) {
    const clover_float x[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    const clover_float y[4] = {p0, p1, p2, p3};
    const clover_float t    = 2.0f + s;
    clover_float result     = 0.0f;
    for (int i = 0; i < 4; ++i) {
        clover_float term = y[i];
        for (int j = 0; j < 4; ++j) {
            if (i != j) {
                term *= (t - x[j]) / (x[i] - x[j]);
            }
        }
        result += term;
    }
    return result;
}

}  // namespace clover::dsp
