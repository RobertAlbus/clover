#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <filesystem>

#include "clover/audio_buffer.hpp"

namespace clover::io {

enum struct audio_file_settings {
    wav_441_16,
    wav_48_24,
    wav_48_32,
    wav_48_64,
    flac_441_16,
    flac_48_24,
    mp3_320,
};

auto sample_rate(audio_file_settings settings) -> int;
struct audio_file_manager {
    auto read(std::filesystem::path path, int as_sample_rate) -> audio_buffer;
    void write(std::filesystem::path path, audio_buffer buffer, audio_file_settings settings);
    void convert_sample_rate(audio_buffer buffer, int sample_rate);
};

}  // namespace clover::io