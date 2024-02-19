#pragma once

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

#include "Clover/Nodes/Adapter/NullAdapter.h"

#include "Clover/Nodes/Basic/Gain.h"

#include "Clover/Nodes/Delay/FractionalDelayLine.h"

// clang-format off
// new envelope ideas
#include "Clover/Nodes/Envelope/EnvelopeStruct.h" // must come first
#include "Clover/Nodes/Envelope/AutomationClip.h" 

// old envelope ideas
#include "Clover/Nodes/Envelope/BasicEnvelope.h"  // must come first
#include "Clover/Nodes/Envelope/ADSR.h"
#include "Clover/Nodes/Envelope/DC.h"
// clang-format on

#include "Clover/Nodes/Filter/EQ.h"
#include "Clover/Nodes/Filter/Filter.h"

#include "Clover/Nodes/StepSequencer/PatternSettable.h"
#include "Clover/Nodes/StepSequencer/StepSequencer.h"
#include "Clover/Nodes/StepSequencer/lib.h"

#include "Clover/Nodes/Stereo/Difference.h"
#include "Clover/Nodes/Stereo/MidSideBalance.h"
#include "Clover/Nodes/Stereo/Pan.h"
#include "Clover/Nodes/Stereo/Sum.h"

#include "Clover/Nodes/Wavetable/WavetableOscInterface.h"

#include "Clover/Nodes/Wavetable/WavetableOsc.h"
#include "Clover/Nodes/Wavetable/WavetableOscStereo.h"

#include "Clover/Nodes/Synth/OscNx.h"