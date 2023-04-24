#pragma once

#include <cmath>
#include <functional>
#include <vector>

#include "Graph.h"

template <typename T> struct STSQ_Pattern {
  STSQ_Pattern() : totalDuration(0) {}

  struct Step {
    T data;
    int start;
  };
  void from(const std::vector<T> &fromSteps, float stepDuration) {
    steps.clear();
    totalDuration = stepDuration * (float)fromSteps.size();

    for (int i = 0; i < fromSteps.size(); i++) {
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
struct STSQ : public Node<0, 0> {

  STSQ() : Node(), nextIndex(0), patternIndex(0) {}

  int patternIndex;
  std::vector<STSQ_Pattern<StepDataType>> patterns;
  std::vector<TargetType *> targets;
  int nextIndex;

  void setPattern(int i) {
    printf("\n ======== ");
    printf("\n setting pattern index to %i", i);
    patternIndex = i;

    updateIndexForNewPattern();
    printf("\n ======== ");
  }

  void addPattern(const STSQ_Pattern<StepDataType> &newPattern) {
    printf("\n adding new pattern");
    patterns.emplace_back(newPattern);
  }

  Frame<0> tick(Frame<0> input) {
    printf("\ncurrent time: %i   ----------------", _currentClockTime);

    checkTimeAndPerformStep();

    return Frame<0>{};
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
    printf("\nelapsed time: %f",
           fmod((float)_currentClockTime, currentPattern().totalDuration));
    return fmod((float)_currentClockTime, currentPattern().totalDuration);
  }

  bool isStartOfNextStep() {
    printf("\nis start of next step?: %d",
           ((int)getElapsedPatternTime()) == currentPattern()[nextIndex].start);
    return ((int)getElapsedPatternTime()) == currentPattern()[nextIndex].start;
  }

  void performStep() {
    printf("\n ---------------- performing step %i at time %i", nextIndex,
           currentPattern()[nextIndex].start);
    applyStepDataFunc(currentPattern()[nextIndex].data, targets);
  }

  bool isStartOfNextIteration() {
    printf("\nis start of next iteration?: %d",
           nextIndex >= currentPattern().size());
    return nextIndex >= currentPattern().size();
  }

  void updateIndexForNewPattern() {
    bool isInitialSampleClockState = _currentClockTime <= 0;
    if (isInitialSampleClockState) {
      printf("\n - at initial clock state, exiting");

      nextIndex = 0;
      return;
    }
    int currentElapsedPatternTime = (int)getElapsedPatternTime();
    printf("\n - current clock time: %i", _currentClockTime);
    printf("\n - total pattern time: %f", currentPattern().totalDuration);
    printf("\n - current elapsed pattern time: %i", currentElapsedPatternTime);
    printf("\n - current pattern has %i steps", currentPattern().size());

    for (int i = 0; i < currentPattern().size() - 1; i++) {
      printf("\n - checking if should start at pattern[%i].start: %i", i,
             currentPattern()[i].start);
      if (currentElapsedPatternTime == currentPattern()[i].start) {
        printf("\n   - exact match");
        nextIndex = i;
        checkTimeAndPerformStep();
        break;
      } else if (currentElapsedPatternTime > currentPattern()[i].start) {
        printf("\n   - matched, keep searching");
        nextIndex = i + 1;
      } else {
        printf("\n   - found, break");
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

// void PitchableFacilitator(const float &data, std::vector<Pitchable>
// &targets);

/*
1

0
10
20
30




*/
