
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


/*******************************************************************/
int main(void);
int main(void)
{
    Interface interface;
    Sine sine;
    sine.freq(100);
    sine.phase(0.);
    Time time(120);

    sine >> interface.rootNode;


    printf("PortAudio Test: output sine wave. SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);

    ScopedPaHandler paInit;
    if( paInit.result() != paNoError ) goto error;

    if (!interface.open(Pa_GetDefaultOutputDevice())) goto done;
    if (!interface.start()) goto close;

    printf("\n\n%f\n", 0);

    while (time.currentUnit(SAMPLE_RATE) <= 5.) {
        float currentSecond = time.currentUnit(SAMPLE_RATE);
        if (fmod((double)currentSecond,0.5) == 0.) {
            sine.freq(sine.freq() * 1.1);
            printf("%f\n", currentSecond);
        }

        // printf("%f - %f - %d\n", 
        //     interface.rootNode.current().getSampleAtIndex(0),
        //     interface.rootNode.current().getSampleAtIndex(1),
        //     time.currentUnit(1)
        // );
    }

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