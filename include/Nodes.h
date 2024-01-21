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

#include "Nodes/Adapter/NullAdapter.h"

#include "Nodes/Basic/Gain.h"

#include "Nodes/Delay/FractionalDelayLine.h"

// clang-format off
// new envelope ideas
#include "Nodes/Envelope/EnvelopeStruct.h" // must come first
#include "Nodes/Envelope/AutomationClip.h" 

// old envelope ideas
#include "Nodes/Envelope/BasicEnvelope.h"  // must come first
#include "Nodes/Envelope/ADSR.h"
#include "Nodes/Envelope/DC.h"
// clang-format on

#include "Nodes/Filter/EQ.h"
#include "Nodes/Filter/Filter.h"

#include "Nodes/StepSequencer/PatternSettable.h"
#include "Nodes/StepSequencer/StepSequencer.h"
#include "Nodes/StepSequencer/lib.h"

#include "Nodes/Stereo/Difference.h"
#include "Nodes/Stereo/MidSideBalance.h"
#include "Nodes/Stereo/Pan.h"
#include "Nodes/Stereo/Sum.h"

#include "Nodes/Wavetable/WavetableOscInterface.h"

#include "Nodes/Wavetable/WavetableOsc.h"
#include "Nodes/Wavetable/WavetableOscStereo.h"

#include "Nodes/Synth/OscNx.h"