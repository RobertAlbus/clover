#pragma once

#include <vector>

#include "StepSequencer.h"

void PatternSettableFacilitator(const int &data,
                                std::vector<PatternSettable *> &targets);

struct STSQ_PatternSettable
    : STSQ<int, PatternSettable, PatternSettableFacilitator> {};
