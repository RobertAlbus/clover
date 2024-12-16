#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/float.hpp"

namespace clover::dsp {

// y = wave_sine(x) for x = [0,2pi)
clover_float wave_sine(clover_float);
// y = wave_square(x) for x = [0,2pi)
clover_float wave_square(clover_float);
// y = wave_saw(x) for x = [0,2pi)
clover_float wave_saw(clover_float);
// y = wave_tri(x) for x = [0,2pi)
clover_float wave_tri(clover_float);
clover_float wave_noise(clover_float);

}  // namespace clover::dsp
