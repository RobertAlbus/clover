// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/env_linear.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/wave.hpp"
#include "clover/io/audio_callback.hpp"
#include "clover/io/audio_file.hpp"
#include "clover/io/stream.hpp"
#include "clover/io/system_audio.hpp"
#include "clover/math.hpp"

bool PLAYBACK = true;
bool RENDER   = true;

using namespace clover;
int main(int argc, char *argv[]) {
    float fs              = 48000;
    int fs_i              = static_cast<int>(fs);
    int duration          = 5 * fs_i;
    int channel_count_out = 2;

    dsp::oscillator osc(fs);
    osc.waveform = dsp::wave_square;
    dsp::oscillator mod(fs);
    mod.waveform = dsp::wave_saw;
    dsp::oscillator noise(fs);
    noise.waveform = dsp::wave_noise;

    dsp::env_linear env;

    auto audio_state_init = [&]() {
        osc.freq(200);
        mod.freq(0);
        osc.phase(0);
        mod.phase(0);
        env.set(0, 9, ((float)duration) * 0.75f);
    };

    auto audio_callback = [&](io::callback_args data) {
        float &L = *(data.output);
        float &R = *(data.output + 1);

        float signal = osc.tick();
        L            = signal;
        R            = signal;

        float freq = frequency_by_octave_difference(400, 3 * (7.f / 12.f) * mod.tick());
        osc.freq(freq);
        mod.freq(env.tick() * (1 + signal));

        if (data.clock_time == duration) {
            return io::callback_status::end;
        }

        return io::callback_status::cont;
    };

    if (PLAYBACK) {
        io::system_audio_config system;
        system.print();
        io::stream stream;
        stream.audio_callback = audio_callback;

        audio_state_init();
        stream.open(io::stream::settings{
                .device_index_in  = system.no_device(),
                .chan_count_in    = 0,
                .device_index_out = system.default_output().index,
                .chan_count_out   = channel_count_out,
                .sample_rate      = fs_i,
                .latency_ms       = 0});

        stream.start();
        stream.wait_to_complete();
        stream.stop();
    }

    if (RENDER) {
        audio_state_init();
        audio_buffer render = io::exec_callback(audio_callback, channel_count_out, fs_i, duration);
        io::audio_file::write("the_sound.wav", render, io::audio_file_settings::wav_441_16);
    }
}
