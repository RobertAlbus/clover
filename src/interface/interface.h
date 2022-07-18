#pragma once

#include "portaudio.h"
#include "sine.h"

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
    int currentSample = 0;
public:
    Sine *rootNode; // TODO: Sine is just a stand-in signal source for now
};
