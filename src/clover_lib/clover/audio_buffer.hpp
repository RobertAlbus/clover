#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <vector>

#include "clover/float.hpp"

namespace clover {

struct audio_buffer {
    std::vector<clover_float> data;

    int channels;
    int sample_rate;
};

void convert_sample_rate(audio_buffer buffer, int sample_rate);

}  // namespace clover