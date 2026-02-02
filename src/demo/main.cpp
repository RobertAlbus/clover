// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/oscillator.hpp"
#include "clover/io/audio_callback.hpp"
#include "clover/io/audio_file.hpp"
#include "clover/io/stream.hpp"
#include "clover/math.hpp"

#include <iostream>

bool PLAYBACK = true;
bool RENDER   = true;

using namespace clover;

int main(int argc, char* argv[]) {
    float fs     = 48000;
    int fs_i     = static_cast<int>(fs);
    int duration = 5 * fs_i;

    dsp::oscillator osc(fs);
    dsp::oscillator mod(fs);

    auto audio_state_init = [&]() {
        osc.freq(400);
        mod.freq(2);
        osc.phase(0);
        mod.phase(0);
    };

    auto audio_callback = [&](io::callback_args args) {
        float& L = args.output[0];
        float& R = args.output[1];

        L = osc.tick();
        R = L;

        float freq = frequency_by_octave_difference(400, (7.f / 12.f) * mod.tick());
        osc.freq(freq);

        if (args.clock_time >= duration) {
            return io::callback_status::end;
        }
        return io::callback_status::cont;
    };

    if (PLAYBACK) {
        audio_state_init();

        io::stream stream({
                .sample_rate  = fs_i,
                .channels_in  = 0,
                .channels_out = 2,
        });

        stream.audio_callback          = audio_callback;
        stream.audio_complete_callback = []() { std::cout << "Playback complete" << std::endl; };

        std::cout << "Starting playback..." << std::endl;
        stream.start();
        stream.wait_to_complete();
        std::cout << "Done" << std::endl;
    }

    if (RENDER) {
        audio_state_init();
        audio_buffer render = io::exec_callback(audio_callback, 2, fs_i, duration);
        io::audio_file::write("test.wav", render, io::audio_file_settings::wav_441_16);
    }
}
