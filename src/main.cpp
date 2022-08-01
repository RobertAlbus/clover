
#define NUM_SECONDS   (90)
#define FRAMES_PER_BUFFER  (64)

#include <stdio.h>
#include <math.h>
#include "portaudio.h"

#include "constants.h"
#include "interface.h"
#include "sine.h"
#include "square.h"
#include "time.h"


class ScopedPaHandler
{
public:
    ScopedPaHandler()
        : _result(Pa_Initialize())
    {
    }
    ~ScopedPaHandler()
    {
        if (_result == paNoError)
        {
            Pa_Terminate();
        }
    }

    PaError result() const { return _result; }

private:
    PaError _result;
};


int main(void)
{
    Time time(120);
    SampleClock clock;
    Interface interface;
    Sine sine;
    float baseSineFreq = 202.;

    Square sq;
    // sq >> sine; // doesn't work, output and input arities are not matched

    sine >> interface.rootNode;

    Sine lfoModulator;
    lfoModulator >> interface.blackHole;
    float lfoModFreqBase = 238.4;
    lfoModulator.freq(lfoModFreqBase);
    float lfoModAmount = 2130;

    Sine lfo;
    float lfoBaseFreq = 666;
    float lfoAmount = 300;
    
    lfo >> interface.blackHole;

    int testQuantity = 0;
    Sine* sineTest[testQuantity];
    for (int i = 0; i < testQuantity; i++)
    {
        sineTest[i] = new Sine();
        *(sineTest[i]) >> interface.blackHole;
    }


    clock.registerTickCallback([&](int currentTime)->void
    {   

        float currentSecond = time.currentUnit(SAMPLE_RATE);
        if (time.currentUnit(1) == 1) {
            printf("\n\n%f\n", 0);
            lfo.freq(837.);
        }
        if (fmod((double)currentSecond,1.) == 0.0) {
            printf("%f\n", currentSecond);
            lfo.freq(837.);
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


    ScopedPaHandler paInit;
    if( paInit.result() != paNoError ) goto error;

    if (!interface.open(Pa_GetDefaultOutputDevice())) goto done;
    if (!interface.start()) goto close;
    
    // TODO: need a way to determine the composition length but this will do for now.
    // I should be able to compute this value once I have composition-level utilities.
    // WEIRD for some reason it is running twice as long as specified
    Pa_Sleep(NUM_SECONDS * 1000);


    interface.stop();

close:
    interface.close();
    goto done;

done:
    printf("Test finished.\n");
    return paNoError;

error:
    fprintf( stderr, "An error occurred while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", paInit.result() );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( paInit.result() ) );
    return 1;
}