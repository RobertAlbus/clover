
#define NUM_SECONDS   (5)
#define FRAMES_PER_BUFFER  (64)

#include <stdio.h>
#include <math.h>
#include "portaudio.h"

#include "constants.h"
#include "interface.h"
#include "sine.h"
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

    sine >> interface.rootNode;

    Sine lfo;
    lfo.freq(700);
    lfo.phase(0.);
    
    lfo >> interface.blackHole;


    clock.registerTickCallback([&](int currentTime)->void
    {   

        if (time.currentUnit(SAMPLE_RATE) <= 10) {
            float baseSineFreq = 500.;
            float currentSecond = time.currentUnit(SAMPLE_RATE);
            if (time.currentUnit(1) == 1) {
                printf("\n\n%f\n", 0);
            }
            if (fmod((double)currentSecond,1.) == 0.0) {
                printf("%f\n", currentSecond);
                lfo.freq(700.);
            } else if (fmod((double)currentSecond,1.) == 0.5) {
                printf("%f\n", currentSecond);
                lfo.freq(600.);
            }

            sine.freq(
                baseSineFreq + ( fmul(100., lfo.current().getSampleAtIndex(0)) )
            );

            if (DEBUG_PRINT) {
                printf("%f - %f - %d\n", 
                    interface.rootNode.current().getSampleAtIndex(0),
                    interface.rootNode.current().getSampleAtIndex(1),
                    (int)time.currentUnit(1)
                );
            }
        }

    });


    ScopedPaHandler paInit;
    if( paInit.result() != paNoError ) goto error;

    if (!interface.open(Pa_GetDefaultOutputDevice())) goto done;
    if (!interface.start()) goto close;
    
    // TODO: need a way to determine the composition length but this will do for now.
    // I should be able to compute this value once I have composition-level utilities.
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