#pragma once

#include "basic/gain.h"
#include "basic/invert.h"
#include "basic/statelessProcessor.h"
#include "basic/subgraph.h"

#include "delay/delay.h" // should be called fractional

#include "envelope/adsr.h"
#include "envelope/dc.h"
#include "envelope/envelope.h"

#include "filter/firLowPass.h"
#include "filter/svf.h"

#include "reduce/bitcrusher.h"
#include "reduce/samplecrusher.h"

#include "stereo/difference.h"
#include "stereo/pan.h"
#include "stereo/peel.h"
#include "stereo/sum.h"

#include "wavetable/wavetableOsc.h"
