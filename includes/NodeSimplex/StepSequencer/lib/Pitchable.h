#pragma once

#include <vector>

#include "../StepSequencer.h"

struct Pitchable {
  virtual void midiNote(float note);
};

void PitchableFacilitator(const float &data, std::vector<Pitchable *> &targets);

struct STSQ_Pitchable : STSQ<float, Pitchable, PitchableFacilitator> {};