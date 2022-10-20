#include <algorithm>
#include <math.h>
#include <stdio.h>
#include "portaudio.h"

#include "interface.h"
#include "rootNode.h"

using namespace Clover::IO;

Interface::Interface() : stream(0)
{
    
}

PaError Interface::initialize()
{
    return resultValidation(Pa_Initialize());
}

PaError Interface::resultValidation(PaError error)
{
    if (error != paNoError) {
        fprintf( stderr, "An error occurred while using the portaudio stream\n" );
        fprintf( stderr, "Error number: %d\n", error );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( error ) );

    }
    return error;
}

PaError Interface::open(PaDeviceIndex index)
{
    PaStreamParameters outputParameters;

    outputParameters.device = index;
    if (outputParameters.device == paNoDevice) {
        return resultValidation(paNoDevice);
    }

    const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(index);
    if (pInfo != 0)
    {
        printf("\nOutput device name: '%s'\r", pInfo->name);
    }

    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    PaError err = Pa_OpenStream(
        &stream,
        NULL, /* no input */
        &outputParameters,
        SAMPLE_RATE,
        paFramesPerBufferUnspecified,
        paClipOff,      /* we won't output out of range samples so don't bother clipping them */
        &Interface::paCallback,
        this            /* Using 'this' for userData so we can cast to Interface* in paCallback method */
        );

    if (err != paNoError)
    {
        /* Failed to open stream to device !!! */
        return resultValidation(err);
    }

    err = Pa_SetStreamFinishedCallback( stream, &Interface::paStreamFinished );

    if (err != paNoError)
    {
        Pa_CloseStream( stream );
        stream = 0;

        return resultValidation(err);
    }

    return paNoError;
}

PaError Interface::close()
{
    if (stream == 0)
        return false;

    PaError err = Pa_CloseStream( stream );
    stream = 0;

    return resultValidation(err);
}


PaError Interface::start()
{
    return resultValidation(Pa_StartStream( stream ));
}

PaError Interface::stop()
{
    return resultValidation(Pa_StopStream( stream ));
}

/* The instance callback, where we have access to every method/variable in object of class Interface */
int Interface::paCallbackMethod(const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags)
{
    float *out = (float*)outputBuffer;
    unsigned long i;

    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;

    for( i=0; i<framesPerBuffer; i++ )
    {
        int currentSample = clock.currentSample();
        rootNode.metaTick(currentSample);
        blackhole1.metaTick(currentSample);
        blackhole2.metaTick(currentSample);

        *out++ = rootNode.frames.current[0];; 
        *out++ = rootNode.frames.current[1];; 
        
        clock.tick();
    }

    return paContinue;

}

/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
int Interface::paCallback( const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData )
{
    /* Here we cast userData to Interface* type so we can call the instance method paCallbackMethod, we can do that since
        we called Pa_OpenStream with 'this' for userData */
    return ((Interface*)userData)->paCallbackMethod(inputBuffer, outputBuffer,
        framesPerBuffer,
        timeInfo,
        statusFlags);
}


void Interface::paStreamFinishedMethod()
{
    printf( "Stream Completed: %s\n", message );
}

/*
    * This routine is called by portaudio when playback is done.
    */
void Interface::paStreamFinished(void* userData)
{
    return ((Interface*)userData)->paStreamFinishedMethod();
}
