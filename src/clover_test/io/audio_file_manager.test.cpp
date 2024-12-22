// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include <gtest/gtest.h>

#include "clover/audio_buffer.hpp"
#include "clover/io/audio_file_manager.hpp"

using namespace clover;
using namespace clover::io;

void delete_if_exists(std::string path) {
    if (std::filesystem::exists(path)) {
        std::filesystem::remove(path);
    }
}

TEST(io_audio_file_manager, writes) {
    audio_buffer buffer;
    buffer.channels    = 1;
    buffer.sample_rate = 48000;

    for (auto _ : std::views::iota(0, 30))
        buffer.data.emplace_back(0);

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

    audio_file_manager file_manager;
    for (auto setting : settings) {
        delete_if_exists(path);

        file_manager.write(path, buffer, setting);
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

    audio_file_manager file_manager;
    delete_if_exists(path);

    buffer.sample_rate = io::sample_rate(audio_file_settings::wav_48_32);

    file_manager.write(path, buffer, audio_file_settings::wav_48_32);
    auto read_buffer = file_manager.read(path, buffer.sample_rate);

    for (auto n : std::views::iota(0, (int)buffer.data.size()))
        // 16 bit: within 1e-4
        // 24 bit: within 1e-6
        // 32 bit: equal
        EXPECT_EQ(buffer.data[n], read_buffer.data[n]);

    EXPECT_EQ(buffer.channels, read_buffer.channels);
    EXPECT_EQ(buffer.sample_rate, read_buffer.sample_rate);

    delete_if_exists(path);
}
