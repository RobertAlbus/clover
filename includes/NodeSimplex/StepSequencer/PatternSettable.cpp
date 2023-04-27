#include <vector>

#include "PatternSettable.h"

void PatternSettableFacilitator(const int &data, std::vector<PatternSettable*> &targets) {
  for (PatternSettable* target : targets)
    target->setPattern(data);
}
