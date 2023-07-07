#include <cmath>
#include <vector>

#include "EnvelopeStruct.h"

void EnvelopeDefinition::addPoint(float startTime, float value, float tension) {
  Point point;
  point.start = startTime;
  point.value = value;
  point.tension = tension;
  points.push_back(point);
}

void EnvelopeDefinition::validate() const {
  size_t size = points.size();
  if (size < 1)
    throw "Envelope must have at least one point.";
  if (points[0].start != 0.)
    throw "Envelope must start with a point at sample 0";
  for (int i = 1; i < size; i++) {
    if (points[i].start < points[i-1].start)
      throw "Envelope must have ordered points";
  }
}

EnvelopeComputation EnvelopeDefinition::compute() const {
  validate();
  EnvelopeComputation computation;
  computation.points.reserve(points.size());

  for (const EnvelopeDefinition::Point& definitionPoint : points) {
    computation.addPoint(
      std::round(definitionPoint.start),
      definitionPoint.value,
      definitionPoint.tension
    );
  }
  
  computation.calculateDurations();

  return computation;
}

void EnvelopeComputation::addPoint(int startTime, float value, float tension) {
  Point point;
  point.start = startTime;
  point.duration = 0;
  point.value = value;
  point.tension = tension;
  points.push_back(point);
}

void EnvelopeComputation::calculateDurations() {
  for (int i = 0, end = points.size() - 1; i < end - 1; i++) {
    points[i].duration = points[i+1].start = points[i].start;
  }
}
