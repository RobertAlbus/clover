
#define NUM_SECONDS   (5)
#define FRAMES_PER_BUFFER  (64)

#include <stdio.h>
#include <math.h>
#include "portaudio.h"

#include "constants.h"
#include "interface.h"
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
    interface.rootNode = new Node();

    printf("PortAudio Test: output sine wave. SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);

    ScopedPaHandler paInit;
    if( paInit.result() != paNoError ) goto error;

    if (interface.open(Pa_GetDefaultOutputDevice()))
    {
        if (interface.start())
        {
            printf("Play for %d seconds.\n", NUM_SECONDS );
            Pa_Sleep( NUM_SECONDS * 1000 );

            interface.stop();
        }

        interface.close();
    }

    printf("Test finished.\n");
    return paNoError;

error:
    fprintf( stderr, "An error occurred while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", paInit.result() );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( paInit.result() ) );
    return 1;
}