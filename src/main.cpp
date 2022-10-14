#define NUM_SECONDS (100)

#include <chrono>
#include <math.h>
#include <numbers>
#include <stdio.h>
#include <thread>

#include <fcntl.h> // swallow portaudio logging

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

int main(int argc, char* argv[])
{
    // // // printf("\nClover Version %d.%d\n", Clover_VERSION_MAJOR, Clover_VERSION_MINOR);
    // // // printf("\nDefault Audio Device Index: %d\n", Pa_GetDefaultOutputDevice());

    // // // Time time(120, SAMPLE_RATE);
    // // // SampleClock clock;
    // // // Interface interface;

    // // // float baseSineFreq = 100.;
    // // // Sine sine;
    // // // sine.freq(baseSineFreq);

    // // // SinDrive<1> sinDrive;
    // // // Pan1 outputPan(0);

    // // // sine >> sinDrive >> outputPan >> interface.rootNode;

    // // // Sine lfo;
    // // // lfo.freq(M_PI_2 * baseSineFreq * 3 * std::numbers::phi);
    // // // lfo.gain(1);


    // // // DC<1> normalizer(0.50001);
    // // // normalizer.gainIn(0.5f);
    // // // Gain<1> g;
    // // // g.gain(0.0001);

    // // // lfo >> normalizer >> interface.blackhole1;

    // // // BasicEnvelope e;
    // // // e.set(0., 1., 1 * time.bar);
    // // // e >> interface.blackhole1;

    // // // outputPan >> *(new WavFile<2>(std::string("test.wav"), 48000)) >> *(new NullAdapter<0,2>()) >> interface.blackhole2;


    // // // clock.registerTickCallback([&](int currentTime)->void
    // // // {   
    // // //     float lfoVal = normalizer.frames.current[0];
    // // //     sinDrive.shape(lfoVal);
    // // //     lfo.freq(M_PI * baseSineFreq * std::numbers::phi * (lfoVal + 3.2));
    // // //     printf("%f - %f\n", lfo.frames.current[0], normalizer.frames.current[0]);

    // // // });


    // // // int saved_stderr = dup(STDERR_FILENO);
    // // // int devnull = open("/dev/null", O_RDWR);
    // // // dup2(devnull, STDERR_FILENO);  // Replace standard out
    // // // if (interface.initialize() != paNoError) return 1;
    // // // dup2(saved_stderr, STDERR_FILENO);

    // // // if (interface.open(Pa_GetDefaultOutputDevice()) != paNoError ) return 1;
    // // // if (interface.start() != paNoError ) {interface.close(); return 1;}

    // // // // TODO: need a way to determine the composition length but this will do for now.
    // // // // I should be able to compute this value once I have composition-level utilities.
    // // // std::this_thread::sleep_for(std::chrono::seconds(NUM_SECONDS));

    // // // interface.stop();
    // // // return paNoError;





    RtMidiIn  *midiin = 0;
    RtMidiOut *midiout = 0;
    // RtMidiIn constructor
    try {
        midiin = new RtMidiIn();
    }
    catch ( RtMidiError &error ) {
        error.printMessage();
        exit( EXIT_FAILURE );
    }
    // Check inputs.
    unsigned int nPorts = midiin->getPortCount();
    std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
    std::string portName;
    for ( unsigned int i=0; i<nPorts; i++ ) {
        try {
        portName = midiin->getPortName(i);
        }
        catch ( RtMidiError &error ) {
        error.printMessage();
        goto cleanup;
        }
        std::cout << "  Input Port #" << i+1 << ": " << portName << '\n';
    }
    // RtMidiOut constructor
    try {
        midiout = new RtMidiOut();
    }
    catch ( RtMidiError &error ) {
        error.printMessage();
        exit( EXIT_FAILURE );
    }
    // Check outputs.
    nPorts = midiout->getPortCount();
    std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";
    for ( unsigned int i=0; i<nPorts; i++ ) {
        try {
        portName = midiout->getPortName(i);
        }
        catch (RtMidiError &error) {
        error.printMessage();
        goto cleanup;
        }
        std::cout << "  Output Port #" << i+1 << ": " << portName << '\n';
    }
    std::cout << '\n';
    // Clean up
    cleanup:
    delete midiin;
    delete midiout;
    return 0;
}
