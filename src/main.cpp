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
using namespace Clover::NodeSimplex::Wavetable;
using namespace Clover::NodeSimplex::Stereo;

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

    Clover::NodeSimplex::Wavetable::Saw osc;
    osc.freq(600);

    osc >> new Clover::NodeSimplex::Stereo::Pan1() >> interface.rootNode;

    Clover::NodeSimplex::Envelope::Adsr adsr(time.bar, 0, 0, time.bar);
    adsr >> blackHole;

    interface.clock.registerTickCallback([&](int currentTime)->void
    {   
        float envelopeValue = adsr.frames.current[0];
        osc.gain(envelopeValue);
        // printf("adsr %f\n", envelopeValue);

        if (fmod(time.currentBar(), 2.f) == 0.) {
            adsr.keyOn();
        } else if (fmod(time.currentBar(), 2.f) == 1.) {
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
