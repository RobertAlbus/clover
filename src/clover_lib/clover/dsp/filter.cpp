// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <cmath>

#include "clover/dsp/filter.hpp"
#include "clover/num.hpp"

namespace clover::dsp {

iir::iir(const iir_coeffs& coeffs) : m_coeffs(coeffs) {
}

/*
Adapted from "Cookbook formulae for audio EQ biquad filter coefficients"
by Robert Bristow-Johnson  <rbj@audioimagination.com>
*/

inline float db_gain(float db) {
    constexpr float ten   = 10;
    constexpr float forty = 40;
    return std::pow(ten, db / forty);
}

iir_coeffs apf(float fs, float f0, float q) {
    float omega     = 2 * num::pi * (f0 / fs);
    float cos_omega = std::cos(omega);
    float sin_omega = std::sin(omega);
    float alpha     = sin_omega / (2 * q);

    float a0 = 1 + alpha;
    float a1 = -2 * cos_omega;
    float a2 = 1 - alpha;

    float b0 = a2;
    float b1 = a1;
    float b2 = a0;

    float normalize = 1 / a0;

    return {.b0 = b0 * normalize,
            .b1 = b1 * normalize,
            .b2 = b2 * normalize,
            .a1 = a1 * normalize,
            .a2 = a2 * normalize};
}

iir_coeffs lpf(float fs, float f0, float q) {
    float omega     = 2 * num::pi * (f0 / fs);
    float cos_omega = std::cos(omega);
    float sin_omega = std::sin(omega);
    float alpha     = sin_omega / (2 * q);

    float a0 = 1 + alpha;
    float a1 = -2 * cos_omega;
    float a2 = 1 - alpha;

    float b1 = (1 - cos_omega);
    float b0 = b1 / 2;
    float b2 = b0;

    float normalize = 1 / a0;

    return {.b0 = b0 * normalize,
            .b1 = b1 * normalize,
            .b2 = b2 * normalize,
            .a1 = a1 * normalize,
            .a2 = a2 * normalize};
}

iir_coeffs hpf(float fs, float f0, float q) {
    float omega     = 2 * num::pi * (f0 / fs);
    float cos_omega = std::cos(omega);
    float sin_omega = std::sin(omega);
    float alpha     = sin_omega / (2 * q);

    float a0 = 1 + alpha;
    float a1 = -2 * cos_omega;
    float a2 = 1 - alpha;

    float b0 = (1 + cos_omega) / 2;
    float b1 = -(1 + cos_omega);
    float b2 = b0;

    float normalize = 1 / a0;

    return {.b0 = b0 * normalize,
            .b1 = b1 * normalize,
            .b2 = b2 * normalize,
            .a1 = a1 * normalize,
            .a2 = a2 * normalize};
}

iir_coeffs bpf(float fs, float f0, float q) {
    float omega     = 2 * num::pi * (f0 / fs);
    float cos_omega = std::cos(omega);
    float sin_omega = std::sin(omega);
    float alpha     = sin_omega / (2 * q);

    float a0 = 1 + alpha;
    float a1 = -2 * cos_omega;
    float a2 = 1 - alpha;

    float b0 = q * alpha;
    float b1 = 0;
    float b2 = -b0;

    float normalize = 1 / a0;

    return {.b0 = b0 * normalize,
            .b1 = b1 * normalize,
            .b2 = b2 * normalize,
            .a1 = a1 * normalize,
            .a2 = a2 * normalize};
}

iir_coeffs notch(float fs, float f0, float q) {
    float omega     = 2 * num::pi * (f0 / fs);
    float cos_omega = std::cos(omega);
    float sin_omega = std::sin(omega);
    float alpha     = sin_omega / (2 * q);

    float a0 = 1 + alpha;
    float a1 = -2 * cos_omega;
    float a2 = 1 - alpha;

    float b0 = 1;
    float b1 = a1;
    float b2 = 1;

    float normalize = 1 / a0;

    return {.b0 = b0 * normalize,
            .b1 = b1 * normalize,
            .b2 = b2 * normalize,
            .a1 = a1 * normalize,
            .a2 = a2 * normalize};
}

iir_coeffs eq(float fs, float f0, float q, float gain) {
    float omega     = 2 * num::pi * (f0 / fs);
    float cos_omega = std::cos(omega);
    float sin_omega = std::sin(omega);
    float alpha     = sin_omega / (2 * q);
    float A         = db_gain(gain);

    float a0 = 1 + (alpha / A);
    float a1 = -2 * cos_omega;
    float a2 = 1 - (alpha / A);

    float b0 = 1 + (alpha * A);
    ;
    float b1 = a1;
    float b2 = 1 - (alpha * A);
    ;

    float normalize = 1 / a0;

    return {.b0 = b0 * normalize,
            .b1 = b1 * normalize,
            .b2 = b2 * normalize,
            .a1 = a1 * normalize,
            .a2 = a2 * normalize};
}

iir_coeffs ls(float fs, float f0, float q, float gain) {
    float omega     = 2 * num::pi * (f0 / fs);
    float cos_omega = std::cos(omega);
    float sin_omega = std::sin(omega);
    float alpha     = sin_omega / (2 * q);
    float A         = db_gain(gain);

    float sqrt_A_alpha = 2 * std::sqrt(A) * alpha;

    float a0 = (A + 1) + (A - 1) * cos_omega + sqrt_A_alpha;
    float a1 = -2 * ((A - 1) + (A + 1) * cos_omega);
    float a2 = (A + 1) + (A - 1) * cos_omega - sqrt_A_alpha;

    float b0 = A * ((A + 1) - (A - 1) * cos_omega + sqrt_A_alpha);
    float b1 = A * 2 * ((A - 1) - (A + 1) * cos_omega);
    float b2 = A * ((A + 1) - (A - 1) * cos_omega - sqrt_A_alpha);

    float normalize = 1 / a0;

    return {.b0 = b0 * normalize,
            .b1 = b1 * normalize,
            .b2 = b2 * normalize,
            .a1 = a1 * normalize,
            .a2 = a2 * normalize};
}

iir_coeffs hs(float fs, float f0, float q, float gain) {
    float omega     = 2 * num::pi * (f0 / fs);
    float cos_omega = std::cos(omega);
    float sin_omega = std::sin(omega);
    float alpha     = sin_omega / (2 * q);
    float A         = db_gain(gain);

    float sqrt_A_alpha = 2 * std::sqrt(A) * alpha;

    float a0 = (A + 1) - (A - 1) * cos_omega + sqrt_A_alpha;
    float a1 = 2 * ((A - 1) - (A + 1) * cos_omega);
    float a2 = (A + 1) - (A - 1) * cos_omega - sqrt_A_alpha;

    float b0 = A * ((A + 1) + (A - 1) * cos_omega + sqrt_A_alpha);
    float b1 = A * -2 * ((A - 1) + (A + 1) * cos_omega);
    float b2 = A * ((A + 1) + (A - 1) * cos_omega - sqrt_A_alpha);

    float normalize = 1 / a0;

    return {.b0 = b0 * normalize,
            .b1 = b1 * normalize,
            .b2 = b2 * normalize,
            .a1 = a1 * normalize,
            .a2 = a2 * normalize};
}

float filter::tick(float x) {
    x = std::clamp(x, -2.f, 2.f);

    float yn = m_coeffs.b0 * x + m_coeffs.b1 * m_inputs[0] + m_coeffs.b2 * m_inputs[1] -
               m_coeffs.a1 * m_outputs[0] - m_coeffs.a2 * m_outputs[1];

    yn = std::clamp(yn, -2.f, 2.f);

    m_inputs[1]  = m_inputs[0];
    m_inputs[0]  = x;
    m_outputs[1] = m_outputs[0];
    m_outputs[0] = yn;

    return yn;
}

std::pair<float, float> filter_2::tick(float in_L, float in_R) {
    in_L = std::clamp(in_L, -2.f, 2.f);
    in_R = std::clamp(in_R, -2.f, 2.f);

    constexpr int L0 = 0;
    constexpr int L1 = 1;
    constexpr int R0 = 2;
    constexpr int R1 = 3;

    float yn_L = m_coeffs.b0 * in_L + m_coeffs.b1 * m_inputs[L0] + m_coeffs.b2 * m_inputs[L1] -
                 m_coeffs.a1 * m_outputs[L0] - m_coeffs.a2 * m_outputs[L1];

    yn_L = std::clamp(yn_L, -2.f, 2.f);

    m_inputs[L1]  = m_inputs[L0];
    m_inputs[L0]  = in_L;
    m_outputs[L1] = m_outputs[L0];
    m_outputs[L0] = yn_L;

    float yn_R = m_coeffs.b0 * in_R + m_coeffs.b1 * m_inputs[R0] + m_coeffs.b2 * m_inputs[R1] -
                 m_coeffs.a1 * m_outputs[R0] - m_coeffs.a2 * m_outputs[R1];

    yn_R = std::clamp(yn_R, -2.f, 2.f);

    m_inputs[R1]  = m_inputs[R0];
    m_inputs[R0]  = in_R;
    m_outputs[R1] = m_outputs[R0];
    m_outputs[R0] = yn_R;

    return {yn_L, yn_R};
}

}  // namespace clover::dsp