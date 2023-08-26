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

#include "NodeSimplex/Adapter/NullAdapter.h"

#include "NodeSimplex/Basic/Gain.h"

#include "NodeSimplex/Delay/FractionalDelayLine.h"

// clang-format off
// new envelope ideas
#include "NodeSimplex/Envelope/EnvelopeStruct.h" // must come first
#include "NodeSimplex/Envelope/AutomationClip.h" 

// old envelope ideas
#include "NodeSimplex/Envelope/BasicEnvelope.h"  // must come first
#include "NodeSimplex/Envelope/ADSR.h"
#include "NodeSimplex/Envelope/DC.h"
// clang-format on

#include "NodeSimplex/Filter/EQ.h"
#include "NodeSimplex/Filter/Filter.h"

#include "NodeSimplex/DynamicRange/AsymClip.h"

#include "NodeSimplex/StepSequencer/PatternSettable.h"
#include "NodeSimplex/StepSequencer/StepSequencer.h"
#include "NodeSimplex/StepSequencer/lib.h"

#include "NodeSimplex/Stereo/Difference.h"
#include "NodeSimplex/Stereo/MidSideBalance.h"
#include "NodeSimplex/Stereo/Pan.h"
#include "NodeSimplex/Stereo/Sum.h"

#include "NodeSimplex/Wavetable/WavetableOscInterface.h"

#include "NodeSimplex/Wavetable/WavetableOsc.h"
#include "NodeSimplex/Wavetable/WavetableOscStereo.h"
