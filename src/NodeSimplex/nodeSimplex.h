#pragma once
#include "basic/statelessProcessor.h"

#include "Adapter/NullAdapter.h"
#include "Adapter/MapAdapter.h"

#include "basic/gain.h"
#include "basic/invert.h"
#include "basic/subgraph.h"

#include "delay/delay.h" // should be called fractional
#include "delay/fractionalDelayLine.h"

#include "envelope/basicEnvelope.h" // must come first
#include "envelope/adsr.h"
#include "envelope/dc.h"

#include "filter/firLowPass.h"
#include "filter/svf.h"

#include "reduce/bitcrusher.h"
#include "reduce/samplecrusher.h"

#include "stereo/difference.h"
#include "stereo/pan.h"
#include "stereo/peel.h"
#include "stereo/sum.h"

#include "waveshape/distExponent.h"
#include "waveshape/ringMod_V1.h"

#include "wavetable/wavetableOsc.h"
