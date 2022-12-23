#define NUM_SECONDS (100)

#include <csignal> // midi input message demo
#include <cmath>

#include <chrono>
#include <cstdlib> // rand
#include <math.h>
#include <numbers>
#include <thread>


#include "portaudio.h"
#include "RtMidi.h"

#include "config.h"

#include "clover.h"


using namespace Clover::IO;
using namespace Clover::NodeSimplex;

float getRandomFloat(int max) {
    return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/max));
}

int main(int argc, char* argv[])
{
////////////////////////////////////////////////////////////////
// CLOVER POC

    printf("\nClover Version %d.%d\n", Clover_VERSION_MAJOR, Clover_VERSION_MINOR);
    printf("\nDefault Audio Device Index: %d\n", Pa_GetDefaultOutputDevice());

    Interface interface;
    Time time(160, SAMPLE_RATE, &interface.clock);
    Clover::NodeSimplex::Adapter::NullAdapter<1,2> blackHole;
    blackHole >> interface.rootNode;


    Wavetable::WavetableOsc osc;
    osc.saw(1024);
    osc.freq(110.);

    Filter::BiQuad<2> filter;
    filter.lowPass();
    filter.set(300., 0.5);

    Stereo::Pan1 pan;
    osc >> pan >> filter >> interface.rootNode;

    Envelope::AdsrSettings adsrSettings(0, time.quat, 0.0f, time.quat);
    Envelope::Adsr adsr(adsrSettings);
    adsr >> blackHole;

    Envelope::DC<1> dc(12.);
    Adapter::MapAdapter<1,4> adapter1;

    dc >> adapter1;

    adapter1.map(0,0);
    adapter1.map(0,3);

    Adapter::MapAdapter<1,4> adapter2(adapter1.settings.current);
    Adapter::NullAdapter<4, 1> nullAdapter;
    adapter1 >> nullAdapter >> blackHole;

    bool isProfilingMode = false;
    if(isProfilingMode)
    {
        int quantity = SAMPLE_RATE * 360 * 10;
        std::vector<Frame<2>> benchmarkData;
        benchmarkData.reserve(quantity);
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < quantity; i++)
        {
            interface.rootNode.metaTick(interface.clock.currentSample());
            interface.clock.tick();
            benchmarkData.emplace_back(interface.rootNode.frames.current);
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = duration_cast<std::chrono::milliseconds>(end - start);
        printf("\n\nTHIS MANY   %i\n\n", duration);
        exit(0);
    }
    else {
        interface.clock.registerTickCallback([&](int currentTime)->void
        {   
            float envelopeValue = adsr.frames.current[0];
            filter.setLowPass(envelopeValue * 10000., 0.8);

            float currentQuat = time.currentQuat();

            if (
                fmod(currentQuat, 8.f) == 0.f ||
                fmod(currentQuat, 8.f) == 3.f ||
                fmod(currentQuat, 8.f) == 6.f
            ) {
                adsr.keyOn();
            }

        });

        if (interface.initialize() != paNoError) return 1;

        if (interface.openDevice(Pa_GetDefaultOutputDevice()) != paNoError ) return 1;
        if (interface.start() != paNoError ) {interface.close(); return 1;}

        // TODO: need a way to determine the composition length but this will do for now.
        // I should be able to compute this value once I have composition-level utilities.
        std::this_thread::sleep_for(std::chrono::seconds(NUM_SECONDS));

        interface.stop();
        return paNoError;
    }
}
