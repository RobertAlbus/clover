#pragma once

#include <algorithm>
#include <math.h>
#include <vector>

#include "Graph.h"

namespace Clover::NodeSimplex::Delay {

struct FractionalDelayLineSettings {
  float delayTime;
  int bufferCapacity;
};

template <size_t __arity>
class FractionalDelayLine
    : public StatefulProcessor<__arity, __arity, FractionalDelayLineSettings> {
public:
  FractionalDelayLine(int bufferCapacity, float delaySamples)
      : StatefulProcessor<__arity, __arity, FractionalDelayLineSettings>(),
        buffer{}, writeHead(0), readHead(0) {

    this->settings.initial.delayTime = delaySamples;
    this->settings.initial.bufferCapacity = bufferCapacity;
    this->settings.reset();

    buffer.reserve(bufferCapacity);
    delay(delaySamples);
  }

  FractionalDelayLine(FractionalDelayLineSettings initialSettings)
      : StatefulProcessor<__arity, __arity, FractionalDelayLineSettings>(
            initialSettings),
        buffer{}, writeHead(0), readHead(0) {
    buffer.reserve(initialSettings.delayTime);
    delay(initialSettings.delayTime);

    this->settings.reset();
  }

  void delay(float time) {
    float bufferSize = (float)this->settings.current.bufferCapacity;
    time = std::clamp(time, 0.f, bufferSize - 1.f);
    this->settings.current.delayTime = time;

    readHead = fmod(((float)writeHead - time + bufferSize), bufferSize);
  }
  float delay() { return this->settings.current.delayTime; }

protected:
  std::vector<AudioFrame<__arity>> buffer;
  float readHead;
  int writeHead;
  AudioFrame<__arity> tick(AudioFrame<__arity> input) {
    int bufferCapacity = this->settings.current.bufferCapacity;

    // push new data to buffer
    buffer[writeHead] = input;

    // lerp
    int previousReadHead = (int)readHead;
    int nextReadHead = (previousReadHead + 1) % bufferCapacity;
    float lerpAmount = readHead - (float)previousReadHead;

    AudioFrame<__arity> f{};
    for (int i = 0; i < __arity; i++) {
      f[i] = std::lerp(buffer[previousReadHead][i], buffer[nextReadHead][i],
                       lerpAmount);
    }

    // advance the buffer
    readHead = fmod(readHead + 1.f, (float)bufferCapacity);
    writeHead = (writeHead + 1) % bufferCapacity;

    return f;
  }
};

} // namespace Clover::NodeSimplex::Delay