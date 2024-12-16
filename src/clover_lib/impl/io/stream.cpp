// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include <cstdio>
#include <fcntl.h>  // swallow portaudio logging
#include <format>   // IWYU pragma: keep
#include <iostream>

#include "portaudio.h"

#include "clover/io/audio_callback.hpp"
#include "clover/io/stream.hpp"

namespace clover::io {

stream::stream() : current_stream(nullptr), completion_signal(0) {
    int saved_stderr = dup(STDERR_FILENO);
    int devnull      = ::open("/dev/null", O_RDWR);

    dup2(devnull, STDERR_FILENO);       // swallow port audio logging
    handle_pa_error(Pa_Initialize());   //
    dup2(saved_stderr, STDERR_FILENO);  // unswallow port audio logging
}
stream::~stream() {
    if (current_stream != nullptr) {
        Pa_StopStream(current_stream);
        Pa_CloseStream(current_stream);
        current_stream = nullptr;
    }

    Pa_Terminate();
}

void stream::start() {
    if (current_stream == nullptr) {
        throw std::runtime_error(
                std::format("clover::stream::start() called before "
                            "clover::stream::open(device)\n"
                            "exiting..."));
    }
    handle_pa_error(Pa_IsStreamActive(current_stream));
    handle_pa_error(Pa_StartStream(current_stream));
}

void stream::wait_to_complete() {
    completion_signal.acquire();
}

void stream::stop() {
    if (current_stream != nullptr) {
        handle_pa_error(Pa_StopStream(current_stream));
        current_stream = nullptr;
    }
}

void stream::open(stream::settings settings) {
    current_settings = settings;
    bool has_input   = current_settings.device_index_in != paNoDevice;
    bool has_output  = current_settings.device_index_out != paNoDevice;

    if (!has_input && !has_output) {
        std::cout << "clover::io::stream::open requires non-zero value for at least one of:\n"
                  << "- chan_count_in\n"
                  << "- chan_count_out" << std::endl;
        return;
    }

    PaStreamParameters *pa_inParams  = nullptr;
    PaStreamParameters *pa_outParams = nullptr;

    if (has_input) {
        pa_inParams               = new PaStreamParameters();
        pa_inParams->channelCount = current_settings.chan_count_in;
        pa_inParams->device = current_settings.device_index_in, pa_inParams->sampleFormat = paFloat32;
        pa_inParams->suggestedLatency          = static_cast<double>(current_settings.latency_ms);
        pa_inParams->hostApiSpecificStreamInfo = nullptr;
    }
    if (has_output) {
        pa_outParams = new PaStreamParameters();

        pa_outParams->channelCount              = current_settings.chan_count_out;
        pa_outParams->device                    = current_settings.device_index_out;
        pa_outParams->sampleFormat              = paFloat32;
        pa_outParams->suggestedLatency          = static_cast<double>(current_settings.latency_ms);
        pa_outParams->hostApiSpecificStreamInfo = nullptr;
    }

    handle_pa_error(Pa_IsFormatSupported(pa_inParams, pa_outParams, current_settings.sample_rate));

    // DID I FIX IT YET?
    // strange behaviour: returning paBadStreamPtr even when successfull
    // - skipping error validation for this reason

    handle_pa_error(Pa_OpenStream(
            &current_stream,
            pa_inParams,
            pa_outParams,
            current_settings.sample_rate,
            paFramesPerBufferUnspecified,
            paNoFlag,
            &stream::pa_audio_callback,
            this));

    handle_pa_error(Pa_SetStreamFinishedCallback(current_stream, &stream::pa_stream_complete_callback));
}

void stream::pa_stream_complete_callback(void *user_data) {
    stream &instance = *(stream *)user_data;

    instance.audio_complete_callback();
    instance.completion_signal.release();
}

int stream::pa_audio_callback(
        // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
        const void *input_buffer,
        void *output_buffer,
        unsigned long frames_per_buffer,
        const PaStreamCallbackTimeInfo *time_info,
        PaStreamCallbackFlags status_flags,
        void *user_data) {
    stream &instance = *(stream *)user_data;

    auto *out       = (float *)output_buffer;
    const float *in = (float *)input_buffer;

    std::fill_n(out, instance.current_settings.chan_count_out * frames_per_buffer, 0.f);

    for (unsigned long i = 0; i < frames_per_buffer; i++) {
        float *current_out     = out;
        callback_status status = instance.audio_callback(callback_args{
                .clock_time     = instance.clock.current_sample(),
                .chan_count_in  = instance.current_settings.chan_count_in,
                .chan_count_out = instance.current_settings.chan_count_out,
                .input          = (float *)in,
                .output         = current_out,
        });

        switch (status) {
            case callback_status::cont:
                in += instance.current_settings.chan_count_in;
                out += instance.current_settings.chan_count_out;
                instance.clock.tick();
                break;
            case callback_status::end:
                return paComplete;
        }
    }

    return paContinue;
}

void stream::handle_pa_error(int err) {
    if (err != paNoError) {
        throw std::runtime_error(std::format(
                "PaErrorCode {} {}\n\n"
                "exiting...\n",
                err,
                Pa_GetErrorText(err)));
    }
}

}  // namespace clover::io