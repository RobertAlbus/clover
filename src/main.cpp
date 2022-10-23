#define NUM_SECONDS (100)

#include <csignal> // midi input message demo

#include <chrono>
#include <cstdlib> // rand
#include <fcntl.h> // swallow portaudio logging
#include <math.h>
#include <numbers>
#include <stdio.h>
#include <thread>


#include "portaudio.h"
#include "RtMidi.h"

#include "config.h"

#include "NodeSimplex/nodeSimplex.h"

#include "constants.h"
#include "IO/interface.h"
#include "IO/midiIn.h"
#include "IO/wavFile.h"
#include "util/musicTime.h"

using namespace Clover::IO;
using namespace Clover::NodeSimplex::Basic;
using namespace Clover::NodeSimplex::Delay;
using namespace Clover::NodeSimplex::Envelope;
using namespace Clover::NodeSimplex::Filter;
using namespace Clover::NodeSimplex::Stereo;
using namespace Clover::NodeSimplex::Wavetable;


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

    MidiIn::printPorts();
    MidiIn midiInput("Axiom A.I.R. Mini32:Axiom A.I.R. Mini32 MIDI 20:0", 1000);
    midiInput.printChange(true);
    midiInput >> new NullAdapter<256,1>() >> interface.blackhole1;

    float baseSineFreq = 60.;
    Square sine;
    sine.freq(baseSineFreq);

    SVF filter(0, 0.7, 1, 1);

    Pan1 outputPan(0);

    sine >> filter >> outputPan >> interface.rootNode;
    outputPan >> *(new WavFile<2>(std::string("test.wav"), 48000)) >> *(new NullAdapter<0,2>()) >> interface.blackhole2;

    srand(11);
    int intervalMultiplier = std::max(rand() / (RAND_MAX / 5), 1);
    float fuck = 0;

    clock.registerTickCallback([&](int currentTime)->void
    {   
        // if (currentTime % (time.quat * intervalMultiplier) == 0) {
        //     printf("\nonChange");

        // }
        float rawMidiSignal = midiInput.frames.current[128+5];
        // printf("\n%f - %f", rawMidiSignal, rawMidiSignal/127);
        filter.q(midiInput.frames.current[128+1] / 127.);
        filter.cutoff(midiInput.frames.current[128+5] / 127.);
        //sine.freq();

    });

    int saved_stderr = dup(STDERR_FILENO);
    int devnull = open("/dev/null", O_RDWR);
    dup2(devnull, STDERR_FILENO);  // Replace standard out
    if (interface.initialize() != paNoError) return 1;
    dup2(saved_stderr, STDERR_FILENO);

    if (interface.open(Pa_GetDefaultOutputDevice()) != paNoError ) return 1;
    if (interface.start() != paNoError ) {interface.close(); return 1;}

    // TODO: need a way to determine the composition length but this will do for now.
    // I should be able to compute this value once I have composition-level utilities.
    std::this_thread::sleep_for(std::chrono::seconds(NUM_SECONDS));

    interface.stop();
    return paNoError;

}
