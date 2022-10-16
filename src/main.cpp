#define NUM_SECONDS (100)

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
// MIDI POC

    // RtMidiIn  *midiin = 0;
    // RtMidiOut *midiout = 0;
    // // RtMidiIn constructor
    // try {
    //     midiin = new RtMidiIn();
    // }
    // catch ( RtMidiError &error ) {
    //     error.printMessage();
    //     exit( EXIT_FAILURE );
    // }
    // // Check inputs.
    // unsigned int nPorts = midiin->getPortCount();
    // std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
    // std::string portName;
    // for ( unsigned int i=0; i<nPorts; i++ ) {
    //     try {
    //     portName = midiin->getPortName(i);
    //     }
    //     catch ( RtMidiError &error ) {
    //     error.printMessage();
    //     goto cleanup;
    //     }
    //     std::cout << "  Input Port #" << i+1 << ": " << portName << '\n';
    // }
    // // RtMidiOut constructor
    // try {
    //     midiout = new RtMidiOut();
    // }
    // catch ( RtMidiError &error ) {
    //     error.printMessage();
    //     exit( EXIT_FAILURE );
    // }
    // // Check outputs.
    // nPorts = midiout->getPortCount();
    // std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";
    // for ( unsigned int i=0; i<nPorts; i++ ) {
    //     try {
    //     portName = midiout->getPortName(i);
    //     }
    //     catch (RtMidiError &error) {
    //     error.printMessage();
    //     goto cleanup;
    //     }
    //     std::cout << "  Output Port #" << i+1 << ": " << portName << '\n';
    // }
    // std::cout << '\n';
    // // Clean up
    // cleanup:
    // delete midiin;
    // delete midiout;
    // // return 0;

////////////////////////////////////////////////////////////////
// CLOVER POC

    printf("\nClover Version %d.%d\n", Clover_VERSION_MAJOR, Clover_VERSION_MINOR);
    printf("\nDefault Audio Device Index: %d\n", Pa_GetDefaultOutputDevice());

    Time time(120, SAMPLE_RATE);
    SampleClock clock;
    Interface interface;

    float baseSineFreq = 200.;
    Sine sine;
    sine.freq(baseSineFreq);

    Sine mod;
    mod.freq(100);

    mod >> *(new NullAdapter<1,2>()) >> interface.rootNode;

    Pan1 outputPan(0);

    sine >> outputPan >> interface.rootNode;
    outputPan >> *(new WavFile<2>(std::string("test.wav"), 48000)) >> *(new NullAdapter<0,2>()) >> interface.blackhole2;

    srand(11);
    int intervalMultiplier = std::max(rand() / (RAND_MAX / 5), 1);
    float fuck = 0;

    clock.registerTickCallback([&](int currentTime)->void
    {   
        if (currentTime % (time.quat * intervalMultiplier) == 0) {
            fuck = getRandomFloat(108) + 80;
            mod.freq(getRandomFloat(16008) + 80);
            intervalMultiplier = std::max(rand() / (RAND_MAX / 5), 1);
            printf("GGGG\n");
        }

        float modAmount = (baseSineFreq - 1) * mod.frames.current[0];
        float original = sine.frames.current[0];
        float normalized = ((sine.frames.current[0] + 1.) / 2.);
        sine.freq((baseSineFreq + modAmount * (original)) + fuck);
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
