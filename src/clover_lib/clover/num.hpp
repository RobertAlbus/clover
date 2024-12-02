#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include <numbers>

#include "clover/float.hpp"

namespace clover {

struct num {
    static constexpr clover_float pi    = std::numbers::pi_v<clover_float>;
    static constexpr clover_float pi_x2 = pi * 2;
};

}  // namespace clover
