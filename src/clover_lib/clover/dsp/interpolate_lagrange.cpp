// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/dsp/interpolate_lagrange.hpp"

namespace clover::dsp {

float interpolate_lagrange(float p0, float p1, float p2, float p3, float s) {
    const float x[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    const float y[4] = {p0, p1, p2, p3};
    const float t    = 2.0f + s;
    float result     = 0.0f;
    for (int i = 0; i < 4; ++i) {
        float term = y[i];
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
