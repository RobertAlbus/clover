#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <array>
#include <cstddef>

#include "clover/dsp/filter.hpp"
#include "clover/float.hpp"

namespace clover::dsp {

template <int channels>
struct filter_n : public iir {
    using frame = std::array<clover_float, static_cast<std::size_t>(2 * channels)>;
    frame tick(frame x) {
    }
};

}  // namespace clover::dsp