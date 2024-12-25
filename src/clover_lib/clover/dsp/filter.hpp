#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <array>

#include "clover/float.hpp"

namespace clover::dsp {

struct iir_coeffs {
    clover_float b0 = 0;
    clover_float b1 = 0;
    clover_float b2 = 0;
    clover_float a1 = 0;
    clover_float a2 = 0;
};

iir_coeffs apf(clover_float fs, clover_float f0, clover_float bw);
iir_coeffs lpf(clover_float fs, clover_float f0, clover_float q);
iir_coeffs hpf(clover_float fs, clover_float f0, clover_float q);
iir_coeffs bpf(clover_float fs, clover_float f0, clover_float q);
iir_coeffs notch(clover_float fs, clover_float f0, clover_float q);
iir_coeffs eq(clover_float fs, clover_float f0, clover_float q, clover_float gain);
iir_coeffs ls(clover_float fs, clover_float f0, clover_float q, clover_float gain);
iir_coeffs hs(clover_float fs, clover_float f0, clover_float q, clover_float gain);

struct iir {
    iir_coeffs m_coeffs = {0, 0, 0, 0, 0};

    iir() = default;
    iir(const iir_coeffs& coeffs);
};

struct filter : public iir {
    using iir::iir;

    std::array<clover_float, 2> m_inputs{0, 0};
    std::array<clover_float, 2> m_outputs{0, 0};

    clover_float tick(clover_float x);
};

struct filter_2 : public iir {
    using iir::iir;

    std::array<clover_float, 4> m_inputs{0, 0, 0, 0};
    std::array<clover_float, 4> m_outputs{0, 0, 0, 0};

    std::array<clover_float, 2> tick(std::array<clover_float, 2> x);
};

}  // namespace clover::dsp