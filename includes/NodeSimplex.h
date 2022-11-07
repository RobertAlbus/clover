#pragma once
#include "NodeSimplex/basic/statelessProcessor.h"

#include "NodeSimplex/Adapter/NullAdapter.h"
#include "NodeSimplex/Adapter/MapAdapter.h"

#include "NodeSimplex/basic/gain.h"
#include "NodeSimplex/basic/invert.h"
#include "NodeSimplex/basic/subgraph.h"

#include "NodeSimplex/delay/delay.h" // should be called fractional
#include "NodeSimplex/delay/fractionalDelayLine.h"

#include "NodeSimplex/envelope/basicEnvelope.h" // must come first
#include "NodeSimplex/envelope/adsr.h"
#include "NodeSimplex/envelope/dc.h"

#include "NodeSimplex/filter/firLowPass.h"
#include "NodeSimplex/filter/svf.h"

#include "NodeSimplex/reduce/bitcrusher.h"
#include "NodeSimplex/reduce/samplecrusher.h"

#include "NodeSimplex/stereo/difference.h"
#include "NodeSimplex/stereo/pan.h"
#include "NodeSimplex/stereo/peel.h"
#include "NodeSimplex/stereo/sum.h"

#include "NodeSimplex/waveshape/distExponent.h"
#include "NodeSimplex/waveshape/ringMod_V1.h"

#include "NodeSimplex/wavetable/wavetableOsc.h"
