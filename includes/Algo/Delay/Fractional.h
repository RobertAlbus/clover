#pragma once

#include <cmath>
#include <vector>

#include "Algo/AlgorithmBase.h"
#include "Util/FloatingPointConcept.h"

namespace Clover::Delay {

template <FloatingPoint T, int __arity>
struct FractionalDelay : public AlgorithmBase<std::array<T, __arity>> {

  typedef std::array<T, __arity> Frame;

  FractionalDelay(int maxDelay) : FractionalDelay(maxDelay, 0) {}

  FractionalDelay(int maxDelay, float delayTime)
      : buffer{}, writeHead(0), bufferCapacity(maxDelay + 1) {
    initializeBuffer();
    delay(delayTime);
  }

  float delay() { return delayTime; }
  void delay(float time) {
    time = std::clamp(time, 0.f, bufferCapacity - 1.f);
    delayTime = time;

    readHead = writeHead - time + bufferCapacity;
    if (readHead >= static_cast<int>(bufferCapacity)) {
      readHead -= bufferCapacity;
    }
  }

  Frame process(Frame input) {
    writeNewFrameToBuffer(input);
    this->processed = buildOutputFrame();
    advanceHeads();

    return this->processed;
  }

protected:
  float readHead;
  int writeHead;
  float delayTime;
  float bufferCapacity;

  std::vector<Frame> buffer;

  void writeNewFrameToBuffer(Frame input) {
    std::copy(std::begin(input), std::end(input),
              std::begin(buffer[writeHead]));
  }

  Frame buildOutputFrame() {
    int previousReadHead = static_cast<int>(readHead);
    float lerpAmount = readHead - (float)previousReadHead;
    int nextReadHead =
        (previousReadHead + 1) % static_cast<int>(bufferCapacity);

    Frame f{};
    for (int i = 0; i < __arity; i++) {
      T previous = buffer[previousReadHead][i];
      T next = buffer[nextReadHead][i];
      f[i] = std::lerp(previous, next, lerpAmount);
    }

    return f;
  }

  void advanceHeads() {
    readHead += 1.f;
    if (readHead >= bufferCapacity) {
      readHead -= bufferCapacity;
    }

    writeHead += 1;
    if (writeHead >= bufferCapacity) {
      writeHead -= bufferCapacity;
    }
  }

  void initializeBuffer() {
    buffer.reserve(bufferCapacity);
    for (int i = 0; i < bufferCapacity; i++) {
      buffer[i] = Frame{};
    }
  }
};

} // namespace Clover::Delay