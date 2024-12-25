// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/dsp/filter.hpp"
#include "clover/float.hpp"
#include "clover/num.hpp"

namespace clover::dsp {

iir::iir(const iir_coeffs& coeffs) : m_coeffs(coeffs) {
}

/*
Adapted from "Cookbook formulae for audio EQ biquad filter coefficients"
by Robert Bristow-Johnson  <rbj@audioimagination.com>
*/

inline clover_float db_gain(clover_float db) {
    constexpr clover_float ten   = 10;
    constexpr clover_float forty = 40;
    return std::pow(ten, db / forty);
}

iir_coeffs apf(clover_float fs, clover_float f0, clover_float q) {
    clover_float omega     = 2 * num::pi * (f0 / fs);
    clover_float cos_omega = std::cos(omega);
    clover_float sin_omega = std::sin(omega);
    clover_float alpha     = sin_omega / (2 * q);

    clover_float a0 = 1 + alpha;
    clover_float a1 = -2 * cos_omega;
    clover_float a2 = 1 - alpha;

    clover_float b0 = a2;
    clover_float b1 = a1;
    clover_float b2 = b0;

    clover_float normalize = 1 / a0;

    return {.b0 = b0 * normalize,
            .b1 = b1 * normalize,
            .b2 = b2 * normalize,
            .a1 = a1 * normalize,
            .a2 = a2 * normalize};
}

iir_coeffs lpf(clover_float fs, clover_float f0, clover_float q) {
    clover_float omega     = 2 * num::pi * (f0 / fs);
    clover_float cos_omega = std::cos(omega);
    clover_float sin_omega = std::sin(omega);
    clover_float alpha     = sin_omega / (2 * q);

    clover_float a0 = 1 + alpha;
    clover_float a1 = -2 * cos_omega;
    clover_float a2 = 1 - alpha;

    clover_float b1 = (1 - cos_omega);
    clover_float b0 = b1 / 2;
    clover_float b2 = b0;

    clover_float normalize = 1 / a0;

    return {.b0 = b0 * normalize,
            .b1 = b1 * normalize,
            .b2 = b2 * normalize,
            .a1 = a1 * normalize,
            .a2 = a2 * normalize};
}

iir_coeffs hpf(clover_float fs, clover_float f0, clover_float q) {
    clover_float omega     = 2 * num::pi * (f0 / fs);
    clover_float cos_omega = std::cos(omega);
    clover_float sin_omega = std::sin(omega);
    clover_float alpha     = sin_omega / (2 * q);

    clover_float a0 = 1 + alpha;
    clover_float a1 = -2 * cos_omega;
    clover_float a2 = 1 - alpha;

    clover_float b0 = (1 + cos_omega) / 2;
    clover_float b1 = -(1 + cos_omega);
    clover_float b2 = b0;

    clover_float normalize = 1 / a0;

    return {.b0 = b0 * normalize,
            .b1 = b1 * normalize,
            .b2 = b2 * normalize,
            .a1 = a1 * normalize,
            .a2 = a2 * normalize};
}

iir_coeffs bpf(clover_float fs, clover_float f0, clover_float q) {
    clover_float omega     = 2 * num::pi * (f0 / fs);
    clover_float cos_omega = std::cos(omega);
    clover_float sin_omega = std::sin(omega);
    clover_float alpha     = sin_omega / (2 * q);

    clover_float a0 = 1 + alpha;
    clover_float a1 = -2 * cos_omega;
    clover_float a2 = 1 - alpha;

    clover_float b0 = q * alpha;
    clover_float b1 = 0;
    clover_float b2 = -b0;

    clover_float normalize = 1 / a0;

    return {.b0 = b0 * normalize,
            .b1 = b1 * normalize,
            .b2 = b2 * normalize,
            .a1 = a1 * normalize,
            .a2 = a2 * normalize};
}

iir_coeffs notch(clover_float fs, clover_float f0, clover_float q) {
    clover_float omega     = 2 * num::pi * (f0 / fs);
    clover_float cos_omega = std::cos(omega);
    clover_float sin_omega = std::sin(omega);
    clover_float alpha     = sin_omega / (2 * q);

    clover_float a0 = 1 + alpha;
    clover_float a1 = -2 * cos_omega;
    clover_float a2 = 1 - alpha;

    clover_float b0 = 1;
    clover_float b1 = a1;
    clover_float b2 = 1;

    clover_float normalize = 1 / a0;

    return {.b0 = b0 * normalize,
            .b1 = b1 * normalize,
            .b2 = b2 * normalize,
            .a1 = a1 * normalize,
            .a2 = a2 * normalize};
}

iir_coeffs eq(clover_float fs, clover_float f0, clover_float q, clover_float gain) {
    clover_float omega     = 2 * num::pi * (f0 / fs);
    clover_float cos_omega = std::cos(omega);
    clover_float sin_omega = std::sin(omega);
    clover_float alpha     = sin_omega / (2 * q);
    clover_float A         = db_gain(gain);

    clover_float a0 = 1 + (alpha / A);
    clover_float a1 = -2 * cos_omega;
    clover_float a2 = 1 - (alpha / A);

    clover_float b0 = 1 + (alpha * A);
    ;
    clover_float b1 = a1;
    clover_float b2 = 1 - (alpha * A);
    ;

    clover_float normalize = 1 / a0;

    return {.b0 = b0 * normalize,
            .b1 = b1 * normalize,
            .b2 = b2 * normalize,
            .a1 = a1 * normalize,
            .a2 = a2 * normalize};
}

iir_coeffs ls(clover_float fs, clover_float f0, clover_float q, clover_float gain) {
    clover_float omega     = 2 * num::pi * (f0 / fs);
    clover_float cos_omega = std::cos(omega);
    clover_float sin_omega = std::sin(omega);
    clover_float alpha     = sin_omega / (2 * q);
    clover_float A         = db_gain(gain);

    clover_float sqrt_A_alpha = 2 * std::sqrt(A) * alpha;

    clover_float a0 = (A + 1) + (A - 1) * cos_omega + sqrt_A_alpha;
    clover_float a1 = -2 * ((A - 1) + (A + 1) * cos_omega);
    clover_float a2 = (A + 1) + (A - 1) * cos_omega - sqrt_A_alpha;

    clover_float b0 = A * ((A + 1) - (A - 1) * cos_omega + sqrt_A_alpha);
    clover_float b1 = A * 2 * ((A - 1) - (A + 1) * cos_omega);
    clover_float b2 = A * ((A + 1) - (A - 1) * cos_omega - sqrt_A_alpha);

    clover_float normalize = 1 / a0;

    return {.b0 = b0 * normalize,
            .b1 = b1 * normalize,
            .b2 = b2 * normalize,
            .a1 = a1 * normalize,
            .a2 = a2 * normalize};
}

iir_coeffs hs(clover_float fs, clover_float f0, clover_float q, clover_float gain) {
    clover_float omega     = 2 * num::pi * (f0 / fs);
    clover_float cos_omega = std::cos(omega);
    clover_float sin_omega = std::sin(omega);
    clover_float alpha     = sin_omega / (2 * q);
    clover_float A         = db_gain(gain);

    clover_float sqrt_A_alpha = 2 * std::sqrt(A) * alpha;

    clover_float a0 = (A + 1) - (A - 1) * cos_omega + sqrt_A_alpha;
    clover_float a1 = 2 * ((A - 1) - (A + 1) * cos_omega);
    clover_float a2 = (A + 1) + (A - 1) * cos_omega - sqrt_A_alpha;

    clover_float b0 = A * ((A + 1) + (A - 1) * cos_omega + sqrt_A_alpha);
    clover_float b1 = A * -2 * ((A - 1) - (A + 1) * cos_omega);
    clover_float b2 = A * ((A + 1) + (A - 1) * cos_omega - sqrt_A_alpha);

    clover_float normalize = 1 / a0;

    return {.b0 = b0 * normalize,
            .b1 = b1 * normalize,
            .b2 = b2 * normalize,
            .a1 = a1 * normalize,
            .a2 = a2 * normalize};
}

clover_float filter::tick(clover_float x) {
    // x *= 0.5;
    clover_float yn = m_coeffs.b0 * x + m_coeffs.b1 * m_inputs[0] + m_coeffs.b2 * m_inputs[1] -
                      m_coeffs.a1 * m_outputs[0] - m_coeffs.a2 * m_outputs[1];

    m_inputs[1]  = m_inputs[0];
    m_inputs[0]  = x;
    m_outputs[1] = m_outputs[0];
    m_outputs[0] = yn;

    return yn;
}

std::array<clover_float, 2> filter_2::tick(std::array<clover_float, 2> x) {
    constexpr int first  = 0;
    constexpr int second = 2;
    constexpr int L      = 0;
    constexpr int R      = 1;

    clover_float yn_L = m_coeffs.b0 * x[L] + m_coeffs.b1 * m_inputs[first + L] +
                        m_coeffs.b2 * m_inputs[second + L] - m_coeffs.a1 * m_outputs[first + L] -
                        m_coeffs.a2 * m_outputs[second + L];

    m_inputs[second + L]  = m_inputs[first + L];
    m_inputs[first + L]   = x[L];
    m_outputs[second + L] = m_outputs[first + L];
    m_outputs[first + L]  = yn_L;

    clover_float yn_R = m_coeffs.b0 * x[R] + m_coeffs.b1 * m_inputs[first + R] +
                        m_coeffs.b2 * m_inputs[second + R] - m_coeffs.a1 * m_outputs[first + R] -
                        m_coeffs.a2 * m_outputs[second + R];

    m_inputs[second + R]  = m_inputs[first + R];
    m_inputs[first + R]   = x[R];
    m_outputs[second + R] = m_outputs[first + R];
    m_outputs[first + R]  = yn_L;

    return {yn_L, yn_R};
}

}  // namespace clover::dsp