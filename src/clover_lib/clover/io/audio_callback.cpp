// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "clover/audio_buffer.hpp"
#include "clover/io/audio_callback.hpp"

namespace clover::io {

audio_buffer exec_callback(callback audio_callback, int channels_out, int sample_rate, int64_t duration) {
    audio_buffer buffer;
    buffer.channels    = channels_out;
    buffer.sample_rate = sample_rate;
    buffer.data.resize(static_cast<size_t>(duration) * channels_out, 0.f);

    for (auto frame : std::views::iota(int64_t{0}, duration)) {
        audio_callback({
                .clock_time   = frame,
                .channels_in  = 0,
                .channels_out = channels_out,
                .input        = nullptr,
                .output       = &(buffer.data[static_cast<size_t>(frame) * channels_out]),
        });
    }
    return buffer;
}

}  // namespace clover::io
