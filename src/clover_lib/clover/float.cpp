// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdlib>

#include "clover/float.hpp"

namespace clover {

bool float_eq(float a, float b) {
    return std::abs(a - b) <= std::numeric_limits<float>::epsilon() * std::abs(a);
}

}  // namespace clover