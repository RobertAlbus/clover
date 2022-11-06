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

#include "NodeSimplex/nodeSimplex.h"

#include "constants.h"
#include "IO/interface.h"
#include "IO/midiIn.h"
#include "IO/wavFile.h"
#include "util/util.h"

using namespace Clover::IO;

float getRandomFloat(int max) {
    return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/max));
}

int main(int argc, char* argv[])
{
////////////////////////////////////////////////////////////////
// CLOVER POC

    printf("\nClover Version %d.%d\n", Clover_VERSION_MAJOR, Clover_VERSION_MINOR);
    printf("\nDefault Audio Device Index: %d\n", Pa_GetDefaultOutputDevice());

    Time time(120, SAMPLE_RATE);
    SampleClock clock;
    Interface interface;

    Clover::NodeSimplex::Wavetable::Saw osc;
    osc.freq(600);

    osc >> new Clover::NodeSimplex::Stereo::Pan1() >> interface.rootNode;

    clock.registerTickCallback([&](int currentTime)->void
    {   
        

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
