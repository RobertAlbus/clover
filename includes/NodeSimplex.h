#pragma once

#include "NodeSimplex/Adapter/NullAdapter.h"
#include "NodeSimplex/Adapter/MapAdapter.h"

#include "NodeSimplex/Basic/Gain.h"
#include "NodeSimplex/Basic/Invert.h"
#include "NodeSimplex/Basic/Rectifier.h"

#include "NodeSimplex/Delay/Delay.h" // should be called fractional
#include "NodeSimplex/Delay/FractionalDelayLine.h"

#include "NodeSimplex/Envelope/BasicEnvelope.h" // must come first
#include "NodeSimplex/Envelope/ADSR.h"
#include "NodeSimplex/Envelope/DC.h"

#include "NodeSimplex/Filter/FirLowPass.h"
#include "NodeSimplex/Filter/BadSVF.h"
#include "NodeSimplex/Filter/BiQuad.h"

#include "NodeSimplex/Range/Avoid.h"
#include "NodeSimplex/Range/Clamp.h"
#include "NodeSimplex/Range/Clip.h"

#include "NodeSimplex/Reduce/Bitcrusher.h"
#include "NodeSimplex/Reduce/Samplecrusher.h"

#include "NodeSimplex/Stereo/Difference.h"
#include "NodeSimplex/Stereo/Invert.h"
#include "NodeSimplex/Stereo/Pan.h"
#include "NodeSimplex/Stereo/Peel.h"
#include "NodeSimplex/Stereo/Sum.h"

#include "NodeSimplex/Waveshape/DistExponent.h"
#include "NodeSimplex/Waveshape/RingMod_V1.h"

#include "NodeSimplex/Wavetable/WavetableOsc.h"
