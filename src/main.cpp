
#define NUM_SECONDS   (5)
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
    float baseSineFreq = 50.;

    Square sq;
    // sq >> sine; // doesn't work, output and input arities are not matched

    sine >> interface.rootNode;

    Sine lfoModulator;
    lfoModulator >> interface.blackHole;
    lfoModulator.freq(120.4);
    float lfoModAmount = 2130;

    Sine lfo;
    float lfoBaseFreq = 666;
    float lfoAmount = 300;
    
    lfo >> interface.blackHole;

    int testQuantity = 200;
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

        lfo.freq( (lfoModulator.current().getSampleAtIndex(0) * lfoModAmount) + lfoBaseFreq);
        sine.freq(
            baseSineFreq + ( lfoAmount * lfo.current().getSampleAtIndex(0) )
        );

        if (DEBUG_PRINT) {
            printf("%f - %f - %d\n", 
                interface.rootNode.current().getSampleAtIndex(0),
                interface.rootNode.current().getSampleAtIndex(1),
                (int)time.currentUnit(1)
            );
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