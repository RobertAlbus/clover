// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <numbers>
#include <ranges>

#include <gtest/gtest.h>

#include "clover/audio_buffer.hpp"
#include "clover/io/audio_file.hpp"

using namespace clover;
using namespace clover::io;

void delete_if_exists(std::string path) {
    if (std::filesystem::exists(path)) {
        std::filesystem::remove(path);
    }
}

TEST(io_audio_file_manager, writes) {
    audio_buffer buffer;
    buffer.channels    = 2;
    buffer.sample_rate = 48000;
    buffer.data.reserve(480000);

    for (auto x : std::views::iota(0, 480000)) {
        float signal = std::sin(float(x)) * 0.8f;
        buffer.data.emplace_back(signal);
        buffer.data.emplace_back(signal);
    }

    std::string path = "audio_file_manager_write.wav";

    std::vector<audio_file_settings> settings{
            audio_file_settings::wav_441_16,
            audio_file_settings::wav_48_24,
            audio_file_settings::wav_48_32,
            audio_file_settings::wav_48_64,
            audio_file_settings::flac_441_16,
            audio_file_settings::flac_48_24,
            audio_file_settings::mp3_320,
    };

    for (auto setting : settings) {
        delete_if_exists(path);

        audio_file::write(path, buffer, setting);
        EXPECT_TRUE(std::filesystem::exists(path));
    }

    delete_if_exists(path);
}

TEST(io_audio_file_manager, writes_reads_correctly) {
    std::string path = "audio_file_manager_read.wav";

    audio_buffer buffer;
    buffer.channels = 2;

    for (auto x : std::views::iota(0, 30))
        buffer.data.emplace_back(static_cast<float>(x) / 30.f);

    delete_if_exists(path);

    buffer.sample_rate = io::sample_rate(audio_file_settings::wav_48_32);

    audio_file::write(path, buffer, audio_file_settings::wav_48_32);
    auto read_buffer = audio_file::read(path, buffer.sample_rate);

    for (auto n : std::views::iota(0, (int)buffer.data.size()))
        // 16 bit: within 1e-4
        // 24 bit: within 1e-6
        // 32 bit: equal
        EXPECT_EQ(buffer.data[n], read_buffer.data[n]);

    EXPECT_EQ(buffer.channels, read_buffer.channels);
    EXPECT_EQ(buffer.sample_rate, read_buffer.sample_rate);

    delete_if_exists(path);
}

TEST(io_audio_file_manager, reads_at_new_sample_rate) {
    std::string path = "audio_file_manager_read_resampled.wav";

    int original_rate  = 48000;
    int requested_rate = 44100;
    int frames         = 48000;

    audio_buffer buffer;
    buffer.channels    = 2;
    buffer.sample_rate = original_rate;
    buffer.data.reserve(static_cast<size_t>(frames) * 2);

    for (auto x : std::views::iota(0, frames)) {
        float signal =
                std::sin(2.f * std::numbers::pi_v<float> * 440.f * float(x) / float(original_rate)) * 0.8f;
        buffer.data.emplace_back(signal);
        buffer.data.emplace_back(signal);
    }

    delete_if_exists(path);
    audio_file::write(path, buffer, audio_file_settings::wav_48_32);

    auto read_buffer = audio_file::read(path, requested_rate);

    EXPECT_EQ(read_buffer.sample_rate, requested_rate);
    EXPECT_EQ(read_buffer.channels, buffer.channels);

    int read_frames     = (int)read_buffer.data.size() / read_buffer.channels;
    int expected_frames = frames * requested_rate / original_rate;
    EXPECT_NEAR(read_frames, expected_frames, 2);

    // resampled data should still be the same sine: RMS of a mid-buffer chunk is 0.8/sqrt(2)
    double sum_squares = 0;
    for (auto n : std::views::iota(10000, 30000)) {
        double sample = read_buffer.data[static_cast<size_t>(n) * 2];
        sum_squares += sample * sample;
    }
    double rms = std::sqrt(sum_squares / 20000.0);
    EXPECT_NEAR(rms, 0.8 / std::numbers::sqrt2, 0.01);

    delete_if_exists(path);
}
