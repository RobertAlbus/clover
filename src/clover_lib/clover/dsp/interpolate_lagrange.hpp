#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/float.hpp"

namespace clover::dsp {

clover_float interpolate_lagrange(
        clover_float p0, clover_float p1, clover_float p2, clover_float p3, clover_float s);

}  // namespace clover::dsp
