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
  for (size_t i = 1; i < size; i++) {
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
  if (points.size() < 2) return;

    size_t end = points.size() - 2;

  for (size_t i = 0; i < end; i++) {
    points[i].duration = points[i+1].start = points[i].start;
  }
}
