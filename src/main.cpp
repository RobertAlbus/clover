#define NUM_SECONDS (100)

#include <chrono>
#include <math.h>
#include <stdio.h>
#include <thread>

#include "portaudio.h"

#include "config.h"

#include "nodeSimplex.h"

#include "constants.h"
#include "interface.h"
#include "time.h"


int main(int argc, char* argv[])
{
    printf("\nClover Version %d.%d\n", Clover_VERSION_MAJOR, Clover_VERSION_MINOR);

    Time time(120, SAMPLE_RATE);
    SampleClock clock;
    Interface interface;

    float baseSineFreq = 400.;
    Sine sine;
    sine.freq(baseSineFreq);
    Pan1 outputPan(0);
    SVF filt(0.3,0.7,1);
    filt.gain = 0.9;

    NoiseWhite LFO;
    LFO.freq(0.0001);
    LFO >> interface.blackhole1;
    float lfoRange = baseSineFreq * 0.01;


    sine >> outputPan >> interface.rootNode;


    // const size_t delayTime = (size_t)((float)SAMPLE_RATE*0.74);
    // Delay<2, delayTime+3000> delay(delayTime+3000);
    // outputPan >> delay >> interface.rootNode;
    // delay >> delay;
    // delay.gain = 0.97;


    // Tri lfoModulator;
    // lfoModulator >> interface.blackhole1;
    // float lfoModFreqBase = 538.4;
    // lfoModulator.freq(lfoModFreqBase);
    // float lfoModAmount = 2130;

    // Saw lfo;
    // float lfoBaseFreq = 66;
    // float lfoAmount = 30;
    
    // lfo >> interface.blackhole1;

    // // Envelope e(-1, 1, (SAMPLE_RATE * 1));
    // Adsr e(20, time.quat * 2., 0, time.quat);
    // e >> interface.blackhole1;

    // int testQuantity = NODE_MAX_INPUT_CAPACITY;
    // Sine* sineTest[testQuantity];
    // for (int i = 0; i < testQuantity; i++)
    // {
    //     sineTest[i] = new Sine();
    //     *(sineTest[i]) >> interface.blackhole1;
    // }


    clock.registerTickCallback([&](int currentTime)->void
    {   
        sine.freq(baseSineFreq + (LFO.getCurrentFrame()[0] * lfoRange));
        printf("%f\n", LFO.getCurrentFrame()[0]);

        // cutLFO.freq(lfoVal * 1111);
        // float currentSecond = time.currentUnit(SAMPLE_RATE);
        
        // // if (time.currentUnit(1) == 1) {
        // //     printf("\n\n%f\n", 0);
        // // }
        // if (fmod(currentSecond,1.) == 0.0) {
        //     printf("%f\n", currentSecond);
        // }


        // lfoModulator.freq(lfoModFreqBase * (sine.frames.current[0] * -0.15));

        // // WOAHJ
        // // lfoModulator.freq(lfoModFreqBase * (sine.frames.current[0] * -0.15) * lfoModulator.frames.current[0]);
        
        // lfo.freq( (lfoModulator.frames.current[0] * lfoModAmount) + lfoBaseFreq /2 + (e.frames.current[0] * 1000));
        // sine.freq(
        //     baseSineFreq
        //     + ( lfoAmount * lfo.frames.current[0] )
        // );
        // sine.gain = e.frames.current[0];

        // if (DEBUG_PRINT) {
        //     printf("%f - %f - %d\n", 
        //         interface.rootNode.frames.current[0],
        //         interface.rootNode.frames.current[1],
        //         (int)time.currentUnit(1)
        //     );
        // }

        // if (fmod((double)currentSecond,4) == 0.0)
        // {
        //     e.keyOn();
        // } else if (fmod((double)currentSecond,4) == 2)
        // {
        //     e.keyOff();
        // }

        // if (fmod((double)currentSecond,16) == 0.0)
        // {
        //     size_t t = (size_t) ((float)delayTime / 2.1);
        //     delay.setDelayTime(delayTime);
        // } else if (fmod((double)currentSecond,16) == 8)
        // {
        //     size_t t = (size_t) ((float)delayTime / 1.1);
        //     delay.setDelayTime(t);
        // }

        // // if (fmod((double)currentSecond,10.) == 0.0) {
        // //     lfoModFreqBase = 238.4; 
        // //     lfoModAmount = 2130;

        // //     lfoBaseFreq = 666;
        // //     lfoAmount = 300;
        // // } else if (fmod((double)currentSecond,10.) == 7.0)
        // // {
        // //     lfoModFreqBase = 119.4;
        // //     lfoModAmount  = (lfoModAmount / 5.) * 6.;
        // //     lfoBaseFreq = (lfoBaseFreq / 5.) * 6.;
        // //     lfoAmount = (lfoAmount / 5.) * 6.;
        // // } else if (fmod((double)currentSecond,10.) == 4.5)
        // // {
        // //     lfoModFreqBase = lfoModFreqBase * 1.1;
        // //     lfoModAmount  = (lfoModAmount / 6.) * 13.;
        // //     lfoBaseFreq = (lfoBaseFreq / 6.) * 7.;
        // //     lfoAmount = ((lfoAmount / 5.) * 7.) + lfoModAmount * 0.25;
        // // }
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
