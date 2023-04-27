#include <cmath>
#include <functional>
#include <vector>

#include "StepSequencer.h"
#include "Exception/NotImplemented.h"

void PatternSettable::setPattern(int i) {
  throw NotImplementedException("PatternSettable:setPattern(int)");
}

void PatternSettableFacilitator(const int &data, std::vector<PatternSettable*> &targets) {
  for (PatternSettable* target : targets)
    target->setPattern(data);
}