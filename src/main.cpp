#define NUM_SECONDS (10)

#include <chrono>
#include <math.h>
#include <stdio.h>
#include <thread>

#include "portaudio.h"

#include "constants.h"
#include "interface.h"
#include "pan1.h"
#include "time.h"
#include "wavetableOsc.h"


int main(void)
{
    Time time(120);
    SampleClock clock;
    Interface interface;

    Sine sine;
    float baseSineFreq = 202.;


    sine >> *(new Pan1(0.2)) >> interface.rootNode;


    Pan1 bhMono;
    Tri lfoModulator;
    lfoModulator >> bhMono >> interface.blackHole;
    float lfoModFreqBase = 238.4;
    lfoModulator.freq(lfoModFreqBase);
    float lfoModAmount = 2130;

    Saw lfo;
    float lfoBaseFreq = 666;
    float lfoAmount = 300;
    
    lfo >> bhMono >> interface.blackHole;

    int testQuantity = NODE_MAX_INPUT_CAPACITY;
    Sine* sineTest[testQuantity];
    for (int i = 0; i < testQuantity; i++)
    {
        sineTest[i] = new Sine();
        *(sineTest[i]) >> bhMono >> interface.blackHole;
    }


    clock.registerTickCallback([&](int currentTime)->void
    {   

        float currentSecond = time.currentUnit(SAMPLE_RATE);
        
        if (time.currentUnit(1) == 1) {
            printf("\n\n%f\n", 0);
        }
        if (fmod(currentSecond,1.) == 0.0) {
            printf("%f\n", currentSecond);
        }


        lfoModulator.freq(lfoModFreqBase * (sine.frames.current[0] * -0.15));

        // WOAHJ
        // lfoModulator.freq(lfoModFreqBase * (sine.frames.current[0] * -0.15) * lfoModulator.frames.current[0]);
        
        lfo.freq( (lfoModulator.frames.current[0] * lfoModAmount) + lfoBaseFreq );
        sine.freq(
            baseSineFreq + ( lfoAmount * lfo.frames.current[0] )
        );


        if (DEBUG_PRINT) {
            printf("%f - %f - %d\n", 
                interface.rootNode.frames.current[0],
                interface.rootNode.frames.current[1],
                (int)time.currentUnit(1)
            );
        }

        if (fmod((double)currentSecond,10.) == 0.0) {
            lfoModFreqBase = 238.4; 
            lfoModAmount = 2130;

            lfoBaseFreq = 666;
            lfoAmount = 300;
        } else if (fmod((double)currentSecond,10.) == 7.0)
        {
            lfoModFreqBase = 119.4;
            lfoModAmount  = (lfoModAmount / 5.) * 6.;
            lfoBaseFreq = (lfoBaseFreq / 5.) * 6.;
            lfoAmount = (lfoAmount / 5.) * 6.;
        } else if (fmod((double)currentSecond,10.) == 4.5)
        {
            lfoModFreqBase = lfoModFreqBase * 1.1;
            lfoModAmount  = (lfoModAmount / 6.) * 13.;
            lfoBaseFreq = (lfoBaseFreq / 6.) * 7.;
            lfoAmount = ((lfoAmount / 5.) * 7.) + lfoModAmount * 0.25;
        }
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
