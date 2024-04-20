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
#include <algorithm>
#include <fcntl.h>

#include <cstring>
#include <exception>
#include <format>
#include <functional>
#include <iostream>
#include <semaphore>
#include <string>
#include <unistd.h>

#include "portaudio.h"

// internal dependencies
#include "Clover/Base/CloverBase.h"
#include "Clover/IO/AudioCallback.h"
#include "Clover/Util/SampleClock.h"

namespace Clover::IO {

struct StreamSettings {
  int deviceIndexIn;
  int numChannelsIn;

  int deviceIndexOut;
  int numChannelsOut;

  int sampleRateHz;
  float suggestedLatencyMs;
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
  int hostIndex;
  std::string hostName;
};

std::string FormatAudioDeviceProperties(const AudioDeviceProperties &props) {
  // clang-format off
  return std::format(""
      "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
      "┃ {}\n"
      "┣━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
      "┃ {:<19} ┃ {:<7}    {:<13} \n"
      "┃ {:<19} ┃ {:<7}    {:<13} \n"
      "┃ {:<19} ┃ {:<7} ms {:<13} \n"
      "┃ {:<19} ┃ {:<7} ms {:<13} \n"
      "┃ {:<19} ┃ {:<7} ms {:<13} \n"
      "┃ {:<19} ┃ {:<7} ms {:<13} \n"
      "┃ {:<19} ┃ {:<7} hz {:<13} \n"
      "┃ {:<19} ┃ {:<7}    {:<13} \n"
      "\n",
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
  // clang-format on
}

struct SystemAudioIoConfig {
  std::vector<std::string> hosts;
  std::vector<AudioDeviceProperties> devices;

  void printDevices() {
    for (auto device : devices) {
      std::cout << FormatAudioDeviceProperties(device);
    }
  }
};

SystemAudioIoConfig GetSystemAudioIoConfig() {
  SystemAudioIoConfig systemConfig{{}, {}};

  int numHostApis = Pa_GetHostApiCount();
  systemConfig.hosts.reserve(numHostApis);

  for (PaHostApiIndex i = 0; i < numHostApis; ++i) {
    systemConfig.hosts.emplace_back(std::string{Pa_GetHostApiInfo(i)->name});
  }

  int numDevices = Pa_GetDeviceCount();
  systemConfig.devices.reserve(numDevices);

  for (PaHostApiIndex i = 0; i < numDevices; ++i) {
    const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
    systemConfig.devices.emplace_back(AudioDeviceProperties{
        deviceInfo->name,
        deviceInfo->maxInputChannels,
        deviceInfo->maxOutputChannels,
        (float)deviceInfo->defaultHighInputLatency,
        (float)deviceInfo->defaultLowInputLatency,
        (float)deviceInfo->defaultHighOutputLatency,
        (float)deviceInfo->defaultLowOutputLatency,
        (float)deviceInfo->defaultSampleRate,
        i,
        deviceInfo->hostApi,
        systemConfig.hosts[deviceInfo->hostApi]});
  }

  return systemConfig;
}

using namespace std;

class Exception : public exception {
public:
  Exception(const char *reasonText) : exception() {
    reason = std::string(reasonText);
  }
  virtual const char *what() { return reason.c_str(); }

private:
  std::string reason;
};

class InterfaceV2 : public Base {
public:
  InterfaceV2() : stream(0), streamSignal(0) {
    int saved_stderr = dup(STDERR_FILENO);
    int devnull = open("/dev/null", O_RDWR);
    dup2(devnull, STDERR_FILENO); // Replace standard out

    throwIfPaException(Pa_Initialize());

    dup2(saved_stderr, STDERR_FILENO);
  }
  ~InterfaceV2() {
    if (stream != 0) {
      throwIfPaException(Pa_StopStream(stream));
      throwIfPaException(Pa_CloseStream(stream));
      stream = 0;
    }

    throwIfPaException(Pa_Terminate());
  }

  void start() {
    if (stream == 0) {
      throw Exception("Clover::IO::InterfaceV2::start called before "
                      "Clover::IO::InterfaceV2::openDevice");
    }
    PaError err = Pa_IsStreamActive(stream);
    if (err) {
      throwIfPaException(err);
      return;
    }
    throwIfPaException(Pa_StartStream(stream));
    Pa_StartStream(stream);
  }

  void waitForStreamComplete() { streamSignal.acquire(); }

  void stop() {
    if (stream != 0) {
      throwIfPaException(Pa_StopStream(stream));
      stream = 0;
    }
  }

  void openDevice(const StreamSettings &streamSettings) {
    currentStreamSettings = streamSettings;
    bool hasInput = streamSettings.deviceIndexIn != paNoDevice;
    bool hasOutput = streamSettings.deviceIndexOut != paNoDevice;

    if (!hasInput && !hasOutput) {
      throw Exception(
          "Clover::IO::InterfaceV2::openDevice requires non-zero channel count "
          "for one of [numChannelsIn | numChannelsOut]"
      );
    }

    PaStreamParameters *pa_inParams = NULL;
    PaStreamParameters *pa_outParams = NULL;

    PaStreamParameters inputParameters;
    if (hasInput) {
      inputParameters.channelCount = streamSettings.numChannelsIn;
      inputParameters.device = streamSettings.deviceIndexIn,
      inputParameters.sampleFormat = paFloat32;
      inputParameters.suggestedLatency =
          static_cast<double>(streamSettings.suggestedLatencyMs);
      inputParameters.hostApiSpecificStreamInfo = NULL;
      pa_inParams = &inputParameters;
    }
    PaStreamParameters outputParameters;
    if (hasOutput) {
      outputParameters.channelCount = streamSettings.numChannelsOut;
      outputParameters.device = streamSettings.deviceIndexOut;
      outputParameters.sampleFormat = paFloat32;
      outputParameters.suggestedLatency =
          static_cast<double>(streamSettings.suggestedLatencyMs);
      outputParameters.hostApiSpecificStreamInfo = NULL;
      pa_outParams = &outputParameters;
    }

    throwIfPaException(Pa_IsFormatSupported(
        pa_inParams, pa_outParams, streamSettings.sampleRateHz
    ));

    PaError err = Pa_OpenStream(
        &stream,
        pa_inParams,
        pa_outParams,
        streamSettings.sampleRateHz,
        paFramesPerBufferUnspecified,
        paNoFlag,
        &InterfaceV2::paCallback,
        this
    );

    if (err != paNoError)
      throwIfPaException(Pa_StopStream(&stream));
    throwIfPaException(err);

    // strange behaviour: returning paBadStreamPtr even when successfull
    // - skipping error validation for this reason
    Pa_SetStreamFinishedCallback(&stream, &InterfaceV2::paStreamFinished);
  }

private:
  static int paCallback(
      const void *inputBuffer,
      void *outputBuffer,
      unsigned long framesPerBuffer,
      const PaStreamCallbackTimeInfo *timeInfo,
      PaStreamCallbackFlags statusFlags,
      void *userData
  ) {

    InterfaceV2 &instance = *(InterfaceV2 *)userData;

    /* Prevent unused variable warnings. */
    (void)timeInfo;
    (void)statusFlags;

    float *out = (float *)outputBuffer;
    const float *in = (float *)inputBuffer;
    
    std::fill_n(out, instance.currentStreamSettings.numChannelsOut * framesPerBuffer, 0.f);

    // TODO: create a toggle to switch to buffered mode vs single-sample mode.
    for (unsigned long i = 0; i < framesPerBuffer; i++) {
      float* currentOut = out;
      AudioCallbackStatus status = instance.audioCallback({
          instance.clock.currentSample(),
          (float *)in,
          currentOut,
          instance.currentStreamSettings.numChannelsIn,
          instance.currentStreamSettings.numChannelsOut,
      });

      switch (status) {
      case AudioCallbackStatus::CONTINUE:
        in += instance.currentStreamSettings.numChannelsIn;
        out += instance.currentStreamSettings.numChannelsOut;
        instance.clock.tick();
        break;
      case AudioCallbackStatus::END:
        instance.audioCompleteCallback();
        instance.streamSignal.release();
        return paComplete;
      }
    }

    return paContinue;
  }

  static void paStreamFinished(void *userData) {
    ((InterfaceV2 *)userData)->audioCompleteCallback();
  }

  void throwIfPaException(PaError err) {
    if (err != paNoError) {
      printf("   %i  ", err);
      throw Exception(Pa_GetErrorText(err));
    }
  }

  std::function<void()> audioCompleteCallback = [&]() {
    printf("STREAM FINISHED\n");
  };
  AudioCallback audioCallback =
      [](AudioCallbackArguments args) -> AudioCallbackStatus {
    throw Exception(
        "Clover::IO::InterfaceV2::audioCallback invoked without being set."
    );
  };

  PaStream *stream;
  StreamSettings currentStreamSettings;
  std::binary_semaphore streamSignal;

public:
  Clover::Util::SampleClock clock;
  void setAudioCallback(
      AudioCallback &&callback
  ) {
    audioCallback = callback;
  }

  void setAudioCompleteCallback(std::function<void()> &&callback) {
    audioCompleteCallback = callback;
  }
};

} // namespace Clover::IO
