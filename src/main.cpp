#define NUM_SECONDS (100)

#include <csignal> // midi input message demo

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
    Time time(120, SAMPLE_RATE, &interface.clock);
    Clover::NodeSimplex::Adapter::NullAdapter<1,2> blackHole;
    blackHole >> interface.rootNode;


    Wavetable::WavetableOsc osc;
    osc.sine(2048);
    osc.freq(110);
    Stereo::Pan1 pan;
    osc >> pan >> interface.rootNode;

    Envelope::AdsrSettings adsrSettings(0, time.beat * 0.1, 1, 0);
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


    // interface.clock.registerTickCallback([&](int currentTime)->void
    // {   
    //     float envelopeValue = adsr.frames.current[0];
    //     osc.gain(envelopeValue * 0.9);
    //     // printf("adsr %f   -   %f\n",2 envelopeValue, osc.frames.current[0]);


    //     if (fmod(time.currentBeat(), 5.f) == 0.) {
    //         adsr.keyOn();
    //     } else if (fmod(time.currentBeat(), 5.f) == 2.) {
    //         adsr.keyOff();
    //     }

    // });

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
    printf("\n\n%i", duration);
    exit(0);

    if (interface.initialize() != paNoError) return 1;

    if (interface.openDevice(Pa_GetDefaultOutputDevice()) != paNoError ) return 1;
    if (interface.start() != paNoError ) {interface.close(); return 1;}

    // TODO: need a way to determine the composition length but this will do for now.
    // I should be able to compute this value once I have composition-level utilities.
    std::this_thread::sleep_for(std::chrono::seconds(NUM_SECONDS));

    interface.stop();
    return paNoError;

}
