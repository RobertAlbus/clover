// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sys/stat.h>

#include "samplerate.h"
#include "sndfile.h"
#include <lame/lame.h>  // available transitively via sndfile

#include "clover/audio_buffer.hpp"
#include "clover/io/audio_file_manager.hpp"

namespace clover::io {

namespace {
    void sndfile_throw_if_error(SNDFILE* file) {
        if (!sf_error(file))
            return;
        if (!!file)
            sf_close(file);
        throw std::runtime_error(sf_strerror(file));
    }

    void write_mp3(std::filesystem::path path, audio_buffer buffer, audio_file_settings settings) {
        lame_t lame = lame_init();
        if (!lame) {
            throw std::runtime_error("lame: could not initialize");
        }

        for (auto sample : buffer.data) {
            if (std::abs(sample) > 1.f) {
                throw std::runtime_error("audio data must be normalized to -1..1 to write with lame");
            }
        }

        lame_set_in_samplerate(lame, buffer.sample_rate);
        lame_set_VBR(lame, vbr_off);
        switch (settings) {
            case audio_file_settings::mp3_320:
                lame_set_brate(lame, 320);
                break;
            default:
                throw std::invalid_argument("io::audio_file_manager - unhandled format in write_mp3");
        }
        lame_set_num_channels(lame, buffer.channels);

        if (lame_init_params(lame) < 0) {
            lame_close(lame);
            throw std::runtime_error("io::audio_file_manager - could not initialize lame");
        }

        const int frame_count = static_cast<int>(buffer.data.size()) / buffer.channels;
        const int bufferSize  = (frame_count * 5 / 4) + 7200;  // x 1.25 + 7200
        std::vector<unsigned char> mp3_buffer(bufferSize);

        int mp3_bytes = lame_encode_buffer_ieee_float(
                lame,
                buffer.data.data(),
                (buffer.channels == 2) ? buffer.data.data() + 1 : nullptr,
                frame_count,
                mp3_buffer.data(),
                static_cast<int>(mp3_buffer.size()));

        if (mp3_bytes < 0) {
            lame_close(lame);
            throw std::runtime_error("io::audio_file_manager - could not write mp3");
        }

        std::ofstream mp3_file(path.c_str(), std::ios::binary);
        mp3_file.write((char*)mp3_buffer.data(), mp3_bytes);

        int flush_bytes = lame_encode_flush(lame, mp3_buffer.data(), static_cast<int>(mp3_buffer.size()));

        mp3_file.write((char*)mp3_buffer.data(), flush_bytes);
        mp3_file.close();

        lame_close(lame);
    }

    void write_pcm(std::filesystem::path path, audio_buffer buffer, audio_file_settings settings) {
        SF_INFO sfinfo;

        sfinfo.channels   = buffer.channels;
        sfinfo.frames     = static_cast<int>(buffer.data.size()) / buffer.channels;
        sfinfo.samplerate = sample_rate(settings);
        sfinfo.format     = 0;

        switch (settings) {
            case audio_file_settings::wav_441_16:
                sfinfo.format = (SF_FORMAT_WAV | SF_FORMAT_PCM_16);
                break;
            case audio_file_settings::wav_48_24:
                sfinfo.format = (SF_FORMAT_WAV | SF_FORMAT_PCM_24);
                break;
            case audio_file_settings::wav_48_32:
                sfinfo.format = (SF_FORMAT_WAV | SF_FORMAT_PCM_32);
                break;
            case audio_file_settings::wav_48_64:
                sfinfo.format = (SF_FORMAT_WAV | SF_FORMAT_DOUBLE);
                break;
            case audio_file_settings::flac_441_16:
                sfinfo.format = (SF_FORMAT_FLAC | SF_FORMAT_PCM_16);
            case audio_file_settings::flac_48_24:
                sfinfo.format = (SF_FORMAT_FLAC | SF_FORMAT_PCM_24);
                break;
            default:
                throw std::invalid_argument("io::audio_file_manager - unhandled format in write_pcm");
        }

        SNDFILE* file = sf_open(path.c_str(), SFM_WRITE, &sfinfo);
        sndfile_throw_if_error(file);

        auto buffer_size = static_cast<sf_count_t>(buffer.data.size());
        sf_count_t count = sf_write_float(file, buffer.data.data(), buffer_size);

        if (count != buffer_size) {
            sndfile_throw_if_error(file);
        }

        sf_write_sync(file);
        sf_close(file);

        sndfile_throw_if_error(file);
    }

}  // namespace

int sample_rate(audio_file_settings settings) {
    switch (settings) {
        case audio_file_settings::flac_441_16:
        case audio_file_settings::wav_441_16:
            return 44100;
            break;
        case audio_file_settings::flac_48_24:
        case audio_file_settings::wav_48_24:
        case audio_file_settings::wav_48_32:
        case audio_file_settings::wav_48_64:
            return 48000;
            break;
        default:
            throw std::invalid_argument("io::audio_file_manager - unhandled format in sample_rate");
    }
}

auto audio_file_manager::read(std::filesystem::path path, int as_sample_rate) -> audio_buffer {
    audio_buffer buffer;
    SF_INFO sfinfo;
    SNDFILE* file = sf_open(path.c_str(), SFM_READ, &sfinfo);
    sndfile_throw_if_error(file);

    buffer.sample_rate = sfinfo.samplerate;
    buffer.channels    = sfinfo.channels;

    buffer.data.resize(sfinfo.frames * sfinfo.channels);
    sf_count_t count = sf_read_float(file, buffer.data.data(), static_cast<sf_count_t>(buffer.data.size()));

    if (count != static_cast<sf_count_t>(buffer.data.size())) {
        sndfile_throw_if_error(file);
    }

    if (!sf_close(file)) {
        sndfile_throw_if_error(file);
    }

    convert_sample_rate(buffer, as_sample_rate);

    return buffer;
}
void audio_file_manager::write(
        std::filesystem::path path, audio_buffer buffer, audio_file_settings settings) {
    std::filesystem::path dir_path = path.parent_path();
    if (!dir_path.empty() && !std::filesystem::exists(dir_path)) {
        std::filesystem::create_directories(dir_path);
    }

    switch (settings) {
        case audio_file_settings::mp3_320:
            write_mp3(path, buffer, settings);
            break;
        case audio_file_settings::flac_441_16:
        case audio_file_settings::flac_48_24:
        case audio_file_settings::wav_441_16:
        case audio_file_settings::wav_48_24:
        case audio_file_settings::wav_48_32:
        case audio_file_settings::wav_48_64:
            write_pcm(path, buffer, settings);
            break;
    }
}

void audio_file_manager::convert_sample_rate(audio_buffer buffer, int sample_rate) {
    if (buffer.sample_rate == sample_rate) {
        return;
    }

    SRC_STATE* libsamplerate_src_state = src_new(SRC_SINC_FASTEST, buffer.channels, nullptr);

    int error = src_error(libsamplerate_src_state);
    if (error) {
        throw std::runtime_error(src_strerror(error));
    }

    float* data_in_start     = buffer.data.data();
    long input_frames_count  = static_cast<long>(buffer.data.size() / buffer.channels);
    double ratio             = static_cast<float>(sample_rate) / static_cast<float>(buffer.sample_rate);
    long output_frames_count = static_cast<long>(static_cast<double>(input_frames_count) * ratio) + 1;

    std::vector<float> output(output_frames_count * buffer.channels);
    float* data_out_start = &output.front();

    SRC_DATA libsampelrate_src_data = {
            .data_in       = data_in_start,
            .data_out      = data_out_start,
            .input_frames  = input_frames_count,
            .output_frames = output_frames_count,
            .src_ratio     = ratio,
    };

    error = src_process(libsamplerate_src_state, &libsampelrate_src_data);
    src_delete(libsamplerate_src_state);
    output.resize(libsampelrate_src_data.output_frames_gen * buffer.channels);

    if (error) {
        throw std::runtime_error(src_strerror(error));
    }
}

}  // namespace clover::io
