#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include <cstdint>

namespace clover_bm {

using bm_int = uint_fast16_t;

constexpr bm_int fs_48k          = 48000;
constexpr bm_int seconds_10      = 10;
constexpr bm_int samples_10s_48k = fs_48k * seconds_10;

}  // namespace clover_bm
