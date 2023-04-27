#include <vector>

#include "../StepSequencer.h"
#include "Exception.h"
#include "Triggerable.h"

void Triggerable::triggerOn() {
  throw NotImplementedException("Triggerable:triggerOn()");
};

void Triggerable::triggerOff() {
  throw NotImplementedException("Triggerable:triggerOff()");
};

void TriggerableFacilitator(const TriggerState &data, std::vector<Triggerable*> &targets) {
  switch (data) {
    case TriggerState::On:
      for (Triggerable* target : targets)
        target->triggerOn();
      break;
    case TriggerState::Off:
      for (Triggerable* target : targets)
        target->triggerOff();
      break;
  }
}