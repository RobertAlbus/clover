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
    sine.gain(1.5);

    Sine sine2;
    sine2.freq(baseSineFreq * 2);

    Pan1 outputPan(0);
    SVF filt(0.3,0.7,1);
    filt.gain(0.9);

    float delayTime = 1; //48000.33 * 4;
    FractionalDelayLine<1,4800000> fdl( delayTime );

    NoiseWhite LFO;
    LFO.freq(0.0001);
    LFO >> interface.blackhole1;
    float lfoRange = baseSineFreq * 0.01;

    Gain1 inverter;
    inverter.gain(1.f);


    sine >> inverter >> outputPan;
    // sine2 >> inverter;
    outputPan.gain(0.5);

    sine >> outputPan >> interface.rootNode;
    sine >> new DC(100.) >> interface.blackhole1;
    new DC(100.) >> interface.blackhole1;

    outputPan >> *(new WavFile<2>(std::string("test.wav"), 48000)) >> *(new NullAdapter<0,2>()) >> interface.blackhole2;


    clock.registerTickCallback([&](int currentTime)->void
    {   
        float lfoNormalized = (LFO.currentFrame()[0] +1.) / 2.;
    });

    printf("\n\n%d\n\n", sizeof(float));
    printf("%d\n\n", sizeof(float));
    printf("%i\n\n", sizeof(float));
    printf("%i\n\n", sizeof(CHAR_BIT));

    if (interface.initialize() != paNoError) return 1;
    if (interface.open(Pa_GetDefaultOutputDevice()) != paNoError ) return 1;
    if (interface.start() != paNoError ) {interface.close(); return 1;}

    // TODO: need a way to determine the composition length but this will do for now.
    // I should be able to compute this value once I have composition-level utilities.
    std::this_thread::sleep_for(std::chrono::seconds(NUM_SECONDS));

    interface.stop();
    return paNoError;
}
