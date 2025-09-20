#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <numbers>


namespace clover {

struct num {
    static constexpr float pi    = std::numbers::pi_v<float>;
    static constexpr float pi_x2 = pi * 2;
};

}  // namespace clover
