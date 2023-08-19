#pragma once

#include <cmath>
#include <functional>
#include <vector>

#include "Graph/NullFrame.h"
#include "Graph/NullNode.h"

struct PatternSettable {
  virtual void setPattern(int i);
};

template <typename T> struct STSQ_Pattern {
  STSQ_Pattern() : totalDuration(0) {}

  struct Step {
    T data;
    int start;
  };
  void from(const std::vector<T> &fromSteps, float stepDuration) {
    steps.clear();
    size_t size = fromSteps.size();
    totalDuration = stepDuration * static_cast<float>(size);

    for (int i = 0, end = size; i < end; i++) {
      add(fromSteps[i], stepDuration * (float)i);
    }
  }

  void add(T data, float startTime) {
    Step step;
    step.data = data;
    step.start = std::round(startTime);
    steps.emplace_back(step);
  }

  int size() { return steps.size(); }

  Step operator[](int i) { return steps[i]; }

  std::vector<Step> steps;
  float totalDuration;
};

template <typename StepDataType, typename TargetType,
          void (*applyStepDataFunc)(const StepDataType &,
                                    std::vector<TargetType *> &)>
struct STSQ : public Clover::Graph::NullNode, public PatternSettable {

  STSQ() : NullNode(), nextIndex(0), patternIndex(0) {}

  int patternIndex;
  std::vector<STSQ_Pattern<StepDataType>> patterns;
  std::vector<TargetType *> targets;
  int nextIndex;

  void setPattern(int i) override {
    // should add bounds checking here
    patternIndex = i;
    updateIndexForNewPattern();
  }

  void addPattern(const STSQ_Pattern<StepDataType> &newPattern) {
    patterns.emplace_back(newPattern);
  }

  Clover::Graph::NullFrame tick(Clover::Graph::NullFrame input) {
    checkTimeAndPerformStep();

    return input;
  }

private:
  STSQ_Pattern<StepDataType> &currentPattern() {
    return patterns[patternIndex];
  }

  void checkTimeAndPerformStep() {
    if (isStartOfNextStep()) {
      performStep();
      incrementStep();
    }
  }

  float getElapsedPatternTime() {
    return fmod((float)_currentClockTime, currentPattern().totalDuration);
  }

  bool isStartOfNextStep() {
    return ((int)getElapsedPatternTime()) == currentPattern()[nextIndex].start;
  }

  void performStep() {
    applyStepDataFunc(currentPattern()[nextIndex].data, targets);
  }

  bool isStartOfNextIteration() { return nextIndex >= currentPattern().size(); }

  void updateIndexForNewPattern() {
    bool isInitialSampleClockState = _currentClockTime <= 0;
    if (isInitialSampleClockState) {
      nextIndex = 0;
      return;
    }
    int currentElapsedPatternTime = (int)getElapsedPatternTime();
    for (int i = 0, end = currentPattern().size() - 1; i < end; i++) {
      if (currentElapsedPatternTime == currentPattern()[i].start) {
        nextIndex = i;
        checkTimeAndPerformStep();
        break;
      } else if (currentElapsedPatternTime > currentPattern()[i].start) {
        nextIndex = i + 1;
      } else {
        break;
      }
    }
  }

  void incrementStep() {
    nextIndex++;
    if (isStartOfNextIteration()) {
      nextIndex = 0;
    }
  }
};
