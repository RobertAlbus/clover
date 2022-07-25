#pragma once

#include "portaudio.h"
#include "node.h"
#include "rootNode.h"
#include "sampleClock.h"
#include "musicTime.h"

class Interface
{
public:
    Interface();
    bool open(PaDeviceIndex index);
    bool close();
    bool start();
    bool stop();

private:
/* The instance callback, where we have access to every method/variable in object of class Interface */
    int paCallbackMethod(   const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags);

    static int paCallback( const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData );
private:
    void paStreamFinishedMethod();
    static void paStreamFinished(void* userData);

    PaStream *stream;
    char message[20];
    SampleClock clock;
public:
    RootNode rootNode;
    RootNode blackHole;
};
