#include "clover/dsp/oscillator.hpp"
#include "clover/io/audio_callback.hpp"
#include "clover/io/stream.hpp"
#include "clover/io/system_audio.hpp"

using namespace clover;

int main(int argc, char *argv[]) {
    float fs     = 48000;
    int fs_i     = static_cast<int>(fs);
    int duration = 2 * fs_i;

    dsp::oscillator osc(fs);
    osc.freq(100);

    auto audioCallback = [&](io::callback_args data) {
        float &L = *(data.output);
        float &R = *(data.output + 1);

        L = osc.tick();
        R = L;

        if (data.clock_time == duration) {
            return io::callback_status::end;
        }
        return io::callback_status::cont;
    };

    io::stream stream;
    io::system_audio_config sys;

    stream.open(io::stream::settings{
            .device_index_in  = -1,
            .chan_count_in    = 0,
            .device_index_out = sys.default_output().index,
            .chan_count_out   = 2,
            .sample_rate      = fs_i,
            .latency_ms       = 0});

    stream.audio_callback = audioCallback;

    stream.start();
    stream.wait_to_complete();
    stream.stop();

    return 0;
}
