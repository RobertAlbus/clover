#pragma once

#include <vector>

struct Triggerable {
  virtual void triggerOn();
  virtual void triggerOff();
};

enum TriggerState { On, Off, Pass };

void TriggerableFacilitator(const TriggerState &data,
                            std::vector<Triggerable *> &targets);