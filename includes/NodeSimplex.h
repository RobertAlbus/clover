#pragma once

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

#include "NodeSimplex/Reduce/Bitcrusher.h"
#include "NodeSimplex/Reduce/Samplecrusher.h"

#include "NodeSimplex/StepSequencer/PatternSettable.h"
#include "NodeSimplex/StepSequencer/StepSequencer.h"
#include "NodeSimplex/StepSequencer/lib.h"

#include "NodeSimplex/Stereo/Difference.h"
#include "NodeSimplex/Stereo/Invert.h"
#include "NodeSimplex/Stereo/Pan.h"
#include "NodeSimplex/Stereo/Stereo.h"
#include "NodeSimplex/Stereo/Sum.h"

#include "NodeSimplex/Waveshape/DistExponent.h"
#include "NodeSimplex/Waveshape/RingMod_V1.h"

#include "NodeSimplex/Wavetable/WavetableOscInterface.h"

#include "NodeSimplex/Wavetable/WavetableOsc.h"
#include "NodeSimplex/Wavetable/WavetableOscStereo.h"
