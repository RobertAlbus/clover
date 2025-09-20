#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.


namespace clover::dsp {

// y = wave_sine(x) for x = [0,2pi)
float wave_sine(float);
// y = wave_square(x) for x = [0,2pi)
float wave_square(float);
// y = wave_saw(x) for x = [0,2pi)
float wave_saw(float);
// y = wave_tri(x) for x = [0,2pi)
float wave_tri(float);
float wave_noise(float);

}  // namespace clover::dsp
