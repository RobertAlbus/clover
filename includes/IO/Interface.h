#pragma once

/*
 * /////////
 * // Clover
 * 
 * Audio processing algorithms and DAG with feedback loops that do not break
 * acyclicity.
 * 
 * Copyright (C) 2023 Rob W. Albus
 *  
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version. This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 */


// swallow portaudio logging
#include <fcntl.h>
#include <thread>

#include "portaudio.h"

// internal dependencies
#include "Base.h"
#include "Interface.h"
#include "RootNode.h"
#include "Util/SampleClock.h"

namespace Clover::IO {

class Interface : public Base {
public:
  Interface() : stream(0) {}

  PaError initialize() {
    int saved_stderr = dup(STDERR_FILENO);
    int devnull = open("/dev/null", O_RDWR);
    dup2(devnull, STDERR_FILENO); // Replace standard out

    return resultValidation(Pa_Initialize());

    dup2(saved_stderr, STDERR_FILENO);
  }

  PaError openDevice(PaDeviceIndex index) {
    PaStreamParameters outputParameters;

    outputParameters.device = index;
    if (outputParameters.device == paNoDevice) {
      return resultValidation(paNoDevice);
    }

    const PaDeviceInfo *pInfo = Pa_GetDeviceInfo(index);
    if (pInfo != 0) {
      printf("\nOutput device name: '%s'\r", pInfo->name);
    }

    outputParameters.channelCount = 2; /* stereo output */
    outputParameters.sampleFormat =
        paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency =
        Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    PaError err = Pa_OpenStream(
        &stream, NULL, /* no input */
        &outputParameters, Base::sampleRate, paFramesPerBufferUnspecified,
        paClipOff, /* we won't output out of range samples so don't bother
                      clipping them */
        &Interface::paCallback, this /* Using 'this' for userData so we can cast
                                        to Interface* in paCallback method */
    );

    if (err != paNoError) {
      /* Failed to open stream to device !!! */
      return resultValidation(err);
    }

    err = Pa_SetStreamFinishedCallback(stream, &Interface::paStreamFinished);

    if (err != paNoError) {
      Pa_CloseStream(stream);
      stream = 0;

      return resultValidation(err);
    }

    return paNoError;
  }

  PaError close() {
    if (stream == 0)
      return false;

    PaError err = Pa_CloseStream(stream);
    stream = 0;

    return resultValidation(err);
  }

  PaError start() { return resultValidation(Pa_StartStream(stream)); }

  PaError stop() { return resultValidation(Pa_StopStream(stream)); }

private:
  PaError resultValidation(PaError error) {
    if (error != paNoError) {
      fprintf(stderr, "An error occurred while using the portaudio stream\n");
      fprintf(stderr, "Error number: %d\n", error);
      fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(error));
    }
    return error;
  }

  /* The instance callback, where we have access to every method/variable in
   * object of class Interface */
  int paCallbackMethod(const void *inputBuffer, void *outputBuffer,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo *timeInfo,
                       PaStreamCallbackFlags statusFlags) {
    float *out = (float *)outputBuffer;
    unsigned long i;

    (void)timeInfo; /* Prevent unused variable warnings. */
    (void)statusFlags;
    (void)inputBuffer;

    for (i = 0; i < framesPerBuffer; i++) {
      *out++ = 0.f;
      *out++ = 0.f;
    }

    for (i = 0; i < framesPerBuffer; i++) {
      --out;
      --out;
    }

    for (i = 0; i < framesPerBuffer; i++) {
      int currentSample = clock.currentSample();
      rootNode.metaTick(currentSample);

      *out++ = rootNode.frames.current[0];
      *out++ = rootNode.frames.current[1];

      clock.tick();
    }

    return paContinue;
  }

  /* This routine will be called by the PortAudio engine when audio is needed.
  ** It may called at interrupt level on some machines so don't do anything
  ** that could mess up the system like calling malloc() or free().
  */
  static int paCallback(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo *timeInfo,
                        PaStreamCallbackFlags statusFlags, void *userData) {
    /* Here we cast userData to Interface* type so we can call the instance
       method paCallbackMethod, we can do that since we called Pa_OpenStream
       with 'this' for userData */
    return ((Interface *)userData)
        ->paCallbackMethod(inputBuffer, outputBuffer, framesPerBuffer, timeInfo,
                           statusFlags);
  }

private:
  void paStreamFinishedMethod() { printf("Stream Completed: %s\n", message); }

  /*
   * This routine is called by portaudio when playback is done.
   */
  static void paStreamFinished(void *userData) {
    return ((Interface *)userData)->paStreamFinishedMethod();
  }

  PaStream *stream;
  char message[20];

public:
  Clover::Util::SampleClock clock;
  RootNode<2> rootNode;
};

} // namespace Clover::IO
