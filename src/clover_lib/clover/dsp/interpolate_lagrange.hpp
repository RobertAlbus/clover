#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>


namespace clover::dsp {

float interpolate_lagrange(float p0, float p1, float p2, float p3, float s);

}  // namespace clover::dsp
