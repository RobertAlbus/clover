#pragma once
#include <cmath>
#include <random>
#include <vector>

#include "Constants.h"
#include "Util/FloatingPointConcept.h"

namespace Clover::Wavetable {

// linear interpolation to create a larger wavetable from a smaller wavetable
// check Wavetable::Generate::Tri to see it in use
template <FloatingPoint T>
std::vector<T> LerpTable(int outputSize, std::vector<T> rawTable) {
  // TODO:
  // Update this to treat the last item in the raw table as the final element,
  // instead of lerping to outputSize+1.
  // This will require updating the usage in the triangle generator.

  T scaleFactor = static_cast<T>(outputSize) / static_cast<T>(rawTable.size());

  std::vector<T> wavetable;
  wavetable.reserve(outputSize);
  for (int i = 0; i < outputSize; i++) {
    T scaledIndex = static_cast<T>(i) / scaleFactor;
    int truncatedIndex = static_cast<int>(scaledIndex);
    int nextIndex = truncatedIndex + 1;
    T weight = scaledIndex - truncatedIndex;

    wavetable.emplace_back(
        std::lerp(rawTable[truncatedIndex], rawTable[nextIndex], weight));
  }
  return wavetable;
}

// Generate a wavetable for a sine wave.
template <FloatingPoint T> std::vector<T> Sine(int size) {
  std::vector<T> wavetable;
  wavetable.reserve(size);
  T size_ = static_cast<T>(size);
  for (int i = 0; i < size; i++) {
    T i_ = static_cast<T>(i);
    wavetable.emplace_back(static_cast<T>(sin((i_ / size_) * M_PI * 2.)));
  }

  return wavetable;
}

// Generate a wavetable for a pulse wave.
// pulseWidth 0..1 inclusive
template <FloatingPoint T> std::vector<T> Pulse(int size, T pulseWidth) {
  pulseWidth = std::clamp(pulseWidth, T(0), T(1));
  size = std::max(1, size);
  T size_ = static_cast<T>(size);
  int positiveRegion = static_cast<int>(size_ * pulseWidth);

  std::vector<T> wavetable;
  wavetable.reserve(size);
  for (int i = 0; i < size; i++) {
    wavetable.emplace_back(i <= positiveRegion ? T(1) : T(-1));
  }

  return wavetable;
}

// Generate a wavetable for a square wave.
template <FloatingPoint T> std::vector<T> Square(int size) {
  return Pulse<T>(size, T(0.5));
}

// Generate a wavetable for a saw wave.
template <FloatingPoint T> std::vector<T> Saw(int size) {
  std::vector<T> wavetable;
  wavetable.reserve(size);

  T sizeMinusOne = static_cast<T>(size - 1);
  for (int i = 0; i < size; i++) {
    T i_ = static_cast<T>(i);
    wavetable.emplace_back(std::lerp(T(1), T(-1), i_ / sizeMinusOne));
  }

  return wavetable;
}

// Generate a wavetable for a triangle wave.
template <FloatingPoint T> std::vector<T> Tri(int size) {
  std::vector<T> lerpTable = {0, 1, 0, -1, 0};
  return LerpTable<T>(size, lerpTable);
}

// Generate a wavetable for a white noise wave.
template <FloatingPoint T> std::vector<T> NoiseWhite(int size) {
  std::vector<T> wavetable;
  wavetable.reserve(size);

  std::default_random_engine generator;
  std::uniform_real_distribution<T> distribution(T(1), T(-1));
  for (int i = 0; i < size; i++) {
    wavetable.emplace_back(distribution(generator));
  }

  return wavetable;
}

} // namespace Clover::Wavetable
