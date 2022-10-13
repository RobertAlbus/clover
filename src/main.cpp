#define NUM_SECONDS (100)

#include <chrono>
#include <math.h>
#include <stdio.h>
#include <thread>

#include <fcntl.h> // swallow portaudio logging

#include "portaudio.h"

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
    printf("\nClover Version %d.%d\n", Clover_VERSION_MAJOR, Clover_VERSION_MINOR);
    printf("\nDefault Audio Device Index: %d\n", Pa_GetDefaultOutputDevice());

    Time time(120, SAMPLE_RATE);
    SampleClock clock;
    Interface interface;

    float baseSineFreq = 100.;
    Sine sine;
    sine.freq(baseSineFreq);

    SinDrive<1> sinDrive;
    Pan1 outputPan(0);

    sine >> sinDrive >> outputPan >> interface.rootNode;

    Sine lfo;
    lfo.freq(0.05);

    DC<1> normalizer(0.5);
    normalizer.gain(0.5);

    lfo >> normalizer >> interface.blackhole1;

    BasicEnvelope e;
    e.set(0., 1., 1 * time.bar);
    e >> interface.blackhole1;

    outputPan >> *(new WavFile<2>(std::string("test.wav"), 48000)) >> *(new NullAdapter<0,2>()) >> interface.blackhole2;


    clock.registerTickCallback([&](int currentTime)->void
    {   
        float lfoVal = normalizer.frames.current[0];
        sinDrive.shape(lfoVal);
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
