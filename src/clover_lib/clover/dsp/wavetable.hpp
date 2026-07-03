#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <vector>

namespace clover::dsp::wavetable {

constexpr std::size_t table_size = 8192;

extern const std::vector<float> sine;
extern const std::vector<float> square;
extern const std::vector<float> saw;
extern const std::vector<float> tri;
extern const std::vector<float> noise;

}  // namespace clover::dsp::wavetable
