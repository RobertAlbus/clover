// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/oscillator.hpp"
#include "clover/io/audio_callback.hpp"
#include "clover/io/stream.hpp"
#include "clover/io/system_audio.hpp"
#include "clover/math.hpp"

using namespace clover;

int main(int argc, char *argv[]) {
    float fs     = 48000;
    int fs_i     = static_cast<int>(fs);
    int duration = 5 * fs_i;

    dsp::oscillator osc(fs);
    osc.freq(400);
    dsp::oscillator mod(fs);
    mod.freq(2);

    auto audioCallback = [&](io::callback_args data) {
        float &L = *(data.output);
        float &R = *(data.output + 1);

        L = osc.tick();
        R = L;

        float freq = frequency_by_octave_difference(400, (7.f / 12.f) * mod.tick());
        osc.freq(freq);

        if (data.clock_time == duration) {
            return io::callback_status::end;
        }
        return io::callback_status::cont;
    };

    io::stream stream;
    io::system_audio_config system;

    system.print();

    stream.open(io::stream::settings{
            .device_index_in  = system.no_device(),
            .chan_count_in    = 0,
            .device_index_out = system.default_output().index,
            .chan_count_out   = 2,
            .sample_rate      = fs_i,
            .latency_ms       = 0});

    stream.audio_callback = audioCallback;

    stream.start();
    stream.wait_to_complete();
    stream.stop();

    return 0;
}
