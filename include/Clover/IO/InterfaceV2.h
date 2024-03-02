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
#include <cstring>
#include <fcntl.h>
#include <format>
#include <functional>
#include <optional>
#include <string>
#include <unistd.h>

#include "portaudio.h"

// internal dependencies
#include "Clover/Base/CloverBase.h"
#include "Clover/Util/SampleClock.h"

namespace Clover::IO {

enum struct AudioCallbackStatus {
  CONTINUE,
  COMPLETE,
  ABORT
};

struct AudioCallbackArguments {
  int currentClockSample;
  const float* input;
  float* output;
  const int numChannelsInput;
  const int numChannelsOutput;
};

struct StreamSettings {
  std::string deviceNameIn;
  std::string deviceNameOut;
  int numChannelsIn;
  int numChannelsOut;
  int sampleRateHz;
  double suggestedLatencyMs;
};

struct AudioDeviceProperties {
  std::string name;
  int maxInputChannels;
  int maxOutputChannels;
  float defaultLatencyMsInputHigh;
  float defaultLatencyMsInputLow;
  float defaultLatencyMsOutputHigh;
  float defaultLatencyMsOutputLow;
  float defaultSampleRate;
  int deviceIndex;
};

std::string formatAudioDeviceProperties(const AudioDeviceProperties& props) {
  size_t borderCount = props.name.size() + 2 ;

  return std::format(""
      "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"
      "┃ {:<46} ┃\n"
      "┣━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
      "┃ {:<19} ┃ {:<7}    {:<13} ┃\n"
      "┃ {:<19} ┃ {:<7}    {:<13} ┃\n"
      "┃ {:<19} ┃ {:<7} ms {:<13} ┃\n"
      "┃ {:<19} ┃ {:<7} ms {:<13} ┃\n"
      "┃ {:<19} ┃ {:<7} ms {:<13} ┃\n"
      "┃ {:<19} ┃ {:<7} ms {:<13} ┃\n"
      "┃ {:<19} ┃ {:<7} hz {:<13} ┃\n"
      "┃ {:<19} ┃ {:<7}    {:<13} ┃\n"
      "┗━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n",
      props.name,
      "Max Channels In", props.maxInputChannels, "",
      "Max Channels Out", props.maxOutputChannels, "",
      "Latency In High", props.defaultLatencyMsInputHigh, "",
      "Latency In Low", props.defaultLatencyMsInputLow, "",
      "Latency Out High", props.defaultLatencyMsOutputHigh, "",
      "Latency Out Low", props.defaultLatencyMsOutputLow, "",
      "Default Sample Rate", static_cast<int>(props.defaultSampleRate), "",
      "Device Index", props.deviceIndex, ""
  );
}

class InterfaceV2 : public Base {
public:
  InterfaceV2() : stream(0) {
    int saved_stderr = dup(STDERR_FILENO);
    int devnull = open("/dev/null", O_RDWR);
    dup2(devnull, STDERR_FILENO); // Replace standard out

    if(resultValidation(Pa_Initialize())) {
      //some exception or otherwise interupt things
    } 

    dup2(saved_stderr, STDERR_FILENO);
  }
  ~InterfaceV2() {
    if (stream != 0) {
      resultValidation(Pa_StopStream(stream));
      resultValidation(Pa_CloseStream(stream));
      stream = 0;
    }

    resultValidation(Pa_Terminate());
  }

  void hostInfo() {
    // TODO
    /*
    - rename this method
    - create method to print device details
    - create format strings
    - print hosts, print devices with details
    */
    printf("\n\nHOST INFO");
    int numHostApis = Pa_GetHostApiCount();
    for (PaHostApiIndex i = 0; i < numHostApis; ++i) {
      const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo(i);
      printf("\nHost %i: %s", i, hostInfo->name);
    }

    printf("\n\nDEVICE INFO");
    int numDevices = Pa_GetDeviceCount();
    for (PaDeviceIndex i = 0; i < numDevices; ++i) {
      const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
      printf("\nDevice %i: %s", i, deviceInfo->name);
    }

    printf("\n\n");
  }

  void openDevice(std::optional<StreamSettings> streamSettings) {
    /*
    - should accept std::optional<StreamSettings> (StreamSettings contract still TBD)
    - build in and out param objects (PaStreamParameters) from this
    - Pa_IsFormatSupported for validation
    - create some abstractions, like std::vector<AudioDeviceProperties> getDevices(); ordered by device index

    */
    
  }

  PaError openDefaultDevice() {
    return openDevice(Pa_GetDefaultOutputDevice());
  }

  PaError openDevice(PaDeviceIndex index) {
    PaStreamParameters outputParameters;

    outputParameters.device = index;
    if (outputParameters.device == paNoDevice) {
      return resultValidation(paNoDevice);
    }

    const PaDeviceInfo *pInfo = Pa_GetDeviceInfo(index);
    if (pInfo != 0) {
      std::string name{pInfo->name};
      printf("\nOutput device name: '%s'\r", name.c_str());
      printf("\n");
    }

    outputParameters.channelCount = 2; /* stereo output */
    outputParameters.sampleFormat =
        paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency =
        Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    PaError err = Pa_OpenStream(
        &stream,
        NULL, /* no input */
        &outputParameters,
        Base::sampleRate,
        paFramesPerBufferUnspecified,
        paNoFlag,
        &InterfaceV2::paCallback,
        this
        /* Using 'this' for userData so we can cast to
           Interface* paCallback method */
    );

    if (err != paNoError) {
      /* Failed to open stream to device !!! */
      return resultValidation(err);
    }

    err = Pa_SetStreamFinishedCallback(stream, &InterfaceV2::paStreamFinished);
    if (err != paNoError) {
      return resultValidation(err);
      Pa_CloseStream(stream);
      stream = 0;
    }

    return paNoError;
  }

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
  int paCallbackMethod(
      const void *inputBuffer,
      void *outputBuffer,
      unsigned long framesPerBuffer,
      const PaStreamCallbackTimeInfo *timeInfo,
      PaStreamCallbackFlags statusFlags
  ) {
    float *out = (float *)outputBuffer;
    float *in = (float *)inputBuffer;

    (void)timeInfo; /* Prevent unused variable warnings. */
    (void)statusFlags;
    (void)inputBuffer;

    for (unsigned long i = 0; i < 2ul * framesPerBuffer; i++) {
      *out++ = 0.f;
    }
    out -= 2ul * framesPerBuffer;

    for (unsigned long i = 0; i < framesPerBuffer; i++) {
      AudioCallbackStatus status = audioCallback({
        clock.currentSample(),
        (float *)in,
        (float *)out,
        streamSettings.numChannelsIn,
        streamSettings.numChannelsOut,
      });

      switch (status) {
        case AudioCallbackStatus::CONTINUE:
          in += streamSettings.numChannelsIn;
          out += streamSettings.numChannelsOut;
          clock.tick();
          break;
        case AudioCallbackStatus::COMPLETE:
          return paComplete;
        case AudioCallbackStatus::ABORT:
          return paAbort;
      }
    }

    return paContinue;
  }

  static int paCallback(
      const void *inputBuffer,
      void *outputBuffer,
      unsigned long framesPerBuffer,
      const PaStreamCallbackTimeInfo *timeInfo,
      PaStreamCallbackFlags statusFlags,
      void *userData
  ) {

    return ((InterfaceV2 *)userData)
        ->paCallbackMethod(
            inputBuffer, outputBuffer, framesPerBuffer, timeInfo, statusFlags
        );
  }

private:
  void paStreamFinishedMethod() { 
    stream = 0;
    printf("Stream Completed: %s\n", message);
  }
  static void paStreamFinished(void *userData) {
    return ((InterfaceV2 *)userData)->paStreamFinishedMethod();
  }

  PaStream *stream;
  char message[20];

public:
  Clover::Util::SampleClock clock;
  StreamSettings streamSettings;
  std::function<AudioCallbackStatus(AudioCallbackArguments)> audioCallback;
};

} // namespace Clover::IO
