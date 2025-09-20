#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <array>


namespace clover::dsp {

struct iir_coeffs {
    float b0 = 0;
    float b1 = 0;
    float b2 = 0;
    float a1 = 0;
    float a2 = 0;
};

iir_coeffs apf(float fs, float f0, float bw);
iir_coeffs lpf(float fs, float f0, float q);
iir_coeffs hpf(float fs, float f0, float q);
iir_coeffs bpf(float fs, float f0, float q);
iir_coeffs notch(float fs, float f0, float q);
iir_coeffs eq(float fs, float f0, float q, float gain);
iir_coeffs ls(float fs, float f0, float q, float gain);
iir_coeffs hs(float fs, float f0, float q, float gain);

struct iir {
    iir_coeffs m_coeffs = {0, 0, 0, 0, 0};

    iir() = default;
    iir(const iir_coeffs& coeffs);
};

struct filter : public iir {
    using iir::iir;

    std::array<float, 2> m_inputs{0, 0};
    std::array<float, 2> m_outputs{0, 0};

    float tick(float x);
};

struct filter_2 : public iir {
    using iir::iir;

    std::array<float, 4> m_inputs{0, 0, 0, 0};
    std::array<float, 4> m_outputs{0, 0, 0, 0};

    std::pair<float, float> tick(float in_L, float in_R);
};

}  // namespace clover::dsp