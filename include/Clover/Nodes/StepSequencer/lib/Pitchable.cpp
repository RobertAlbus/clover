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
#include "Pitchable.h"

void Pitchable::midiNote(float note) {
  throw NotImplementedException("Pitchable:pitch(float)");
};

void PitchableFacilitator(
    const float &data, std::vector<Pitchable *> &targets
) {
  for (Pitchable *target : targets)
    target->midiNote(data);
}
