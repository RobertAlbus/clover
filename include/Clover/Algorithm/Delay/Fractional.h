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

#include <algorithm>
#include <cmath>
#include <vector>

#include "Clover/Util/FloatingPointConcept.h"

namespace Clover::Delay {

template <FloatingPoint T, int __arity> struct FractionalDelay {

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

    Frame f{};
    int previousReadHead = static_cast<int>(readHead);
    float lerpAmount = readHead - (float)previousReadHead;
    int nextReadHead = previousReadHead + 1;
    if (nextReadHead >= bufferCapacity) {
      nextReadHead -= bufferCapacity;
    }

    for (size_t i = 0; i < __arity; i++) {
      T previous = buffer[previousReadHead][i];
      T next = buffer[nextReadHead][i];
      f[i] = std::lerp(previous, next, lerpAmount);
    }

    readHead += 1.f;
    if (readHead >= bufferCapacity) {
      readHead -= bufferCapacity;
    }

    writeHead += 1;
    if (writeHead >= bufferCapacity) {
      writeHead -= bufferCapacity;
    }

    return f;
  }

protected:
  float readHead;
  int writeHead;
  float delayTime;
  float bufferCapacity;

  std::vector<Frame> buffer;

  void writeNewFrameToBuffer(Frame &input) {
    std::copy(
        std::begin(input), std::end(input), std::begin(buffer[writeHead])
    );
  }

  void initializeBuffer() {
    buffer.reserve(bufferCapacity);
    for (int i = 0; i < bufferCapacity; i++) {
      buffer[i] = Frame{};
    }
  }
};

} // namespace Clover::Delay
