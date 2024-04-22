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

#include <vector>

#include "../StepSequencer.h"
#include "Clover/Exception/NotImplemented.h"
#include "Triggerable.h"

void Triggerable::triggerOn() {
  throw NotImplementedException("Triggerable:triggerOn()");
};

void Triggerable::triggerOff() {
  throw NotImplementedException("Triggerable:triggerOff()");
};

void TriggerableFacilitator(
    const TriggerState &data, std::vector<Triggerable *> &targets
) {
  switch (data) {
  case TriggerState::On:
    for (Triggerable *target : targets)
      target->triggerOn();
    break;
  case TriggerState::Off:
    for (Triggerable *target : targets)
      target->triggerOff();
    break;
  case TriggerState::Pass:
    break;
  default:
    break;
  }
}
