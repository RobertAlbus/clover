#include <vector>

#include "../StepSequencer.h"
#include "Exception.h"
#include "Pitchable.h"

void Pitchable::midiNote(float note) {
  throw NotImplementedException("Pitchable:pitch(float)");
};

void PitchableFacilitator(const float &data, std::vector<Pitchable*> &targets) {
  for (Pitchable* target : targets)
    target->midiNote(data);
}
