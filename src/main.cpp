#define NUM_SECONDS (100)

#include <chrono>
#include <math.h>
#include <stdio.h>
#include <thread>

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

    if (interface.initialize() != paNoError) return 1;
    if (interface.open(Pa_GetDefaultOutputDevice()) != paNoError ) return 1;
    if (interface.start() != paNoError ) {interface.close(); return 1;}

    // TODO: need a way to determine the composition length but this will do for now.
    // I should be able to compute this value once I have composition-level utilities.
    std::this_thread::sleep_for(std::chrono::seconds(NUM_SECONDS));

    interface.stop();
    return paNoError;
}
