#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cstdint>
#include <functional>

#include "clover/audio_buffer.hpp"

namespace clover::io {

struct callback_args {
    const int64_t clock_time;
    const int channels_in;
    const int channels_out;
    const float* input;  // nullptr if channels_in == 0
    float* output;       // nullptr if channels_out == 0
};

enum class callback_status { cont, end };

using callback          = std::function<callback_status(callback_args)>;
using complete_callback = std::function<void()>;

audio_buffer exec_callback(callback audio_callback, int channels_out, int sample_rate, int64_t duration);

}  // namespace clover::io
