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
    osc.saw();
    osc.freq(60);
    Stereo::Pan1 pan;
    osc >> pan >> interface.rootNode;

    Delay::FractionalDelayLine<1, 48000> delay(*(new Delay::FractionalDelayLineSettings(47777)));
    Basic::Gain<1> fbGain;
    Basic::Gain<1> delayOutGain;
    fbGain.gain(0.8);
    delayOutGain.gain(0.8);

    osc >> delay >> fbGain >> delay;
    fbGain >> delayOutGain >> pan;

    Envelope::AdsrSettings adsrSettings(10, time.beat * 0.1, 0, 0);
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

    interface.clock.registerTickCallback([&](int currentTime)->void
    {   
        float envelopeValue = adsr.frames.current[0];
        osc.gain(envelopeValue);
        // printf("adsr %f\n", envelopeValue);

        if (fmod(time.currentBeat(), 5.f) == 0.) {
            adsr.keyOn();
        } else if (fmod(time.currentBeat(), 5.f) == 1.) {
            adsr.keyOff();
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
