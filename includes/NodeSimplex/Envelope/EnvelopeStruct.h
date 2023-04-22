#pragma once

#include <vector>

struct EnvelopeComputation {
  struct Point {
    int start;
    int duration;
    float value;
    float tension;
  };

  std::vector<Point> points;

  void addPoint(int startTime, float value, float tension);

  void calculateDurations();
};

struct EnvelopeDefinition {
  struct Point {
    float start;
    float value;
    float tension;
  };

  std::vector<Point> points;

  void addPoint(float startTime, float value, float tension = 0.f);

  void validate() const;

  EnvelopeComputation compute() const;
};
