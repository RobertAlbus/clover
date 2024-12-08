#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include <functional>
#include <memory>

#include "clover/dsp/phase_increment_tracker.hpp"
#include "clover/float.hpp"

namespace clover::dsp {

clover_float wave_sine(clover_float);
clover_float wave_square(clover_float);
clover_float wave_saw(clover_float);
clover_float wave_tri(clover_float);
clover_float wave_noise(clover_float);

}  // namespace clover::dsp
