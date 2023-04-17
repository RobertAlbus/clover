#include <cmath>

#include "IIR_Coefficients.h"
#include "Util.h"

template <typename T>
IIRFilterCoefficients<T> SecondOrderResonantButterworthLowPass(T cutoff_freq, T Q, T samplerate) {
    T w0 = 2.0 * M_PI * cutoff_freq / samplerate;
    T alpha = sin(w0) / (2.0 * Q);

    T b0 = w0*w0 / (1.0 + 2.0*alpha + w0*w0);
    T b1 = 2.0*b0;
    T b2 = b0;
    T a1 = 2.0 * (w0*w0 - 1.0) / (1.0 + 2.0*alpha + w0*w0);
    T a2 = (1.0 - 2.0*alpha + w0*w0) / (1.0 + 2.0*alpha + w0*w0);

    return IIRFilterCoefficients<T>{1.0, b0, b1, b2, a1, a2};
}

template <typename T>
IIRFilterCoefficients<T> SecondOrderResonantButterworthHighPass(T cutoff_freq, T Q, T samplerate) {
    T w0 = 2.0 * M_PI * cutoff_freq / samplerate;
    T alpha = sin(w0) / (2.0 * Q);

    T b0 = (1.0 + cos(w0)) / 2.0;
    T b1 = -(1.0 + cos(w0));
    T b2 = b0;
    T a1 = 2.0 * (1.0 - w0*w0) / (1.0 + 2.0*alpha + w0*w0);
    T a2 = (1.0 - 2.0*alpha + w0*w0) / (1.0 + 2.0*alpha + w0*w0);

    return IIRFilterCoefficients<T>{1.0, b0, b1, b2, a1, a2};
}

template <FloatingPoint T>
IIRFilterCoefficients<T> SecondOrderButterworthResonantBandPass(T center_freq, T Q, T samplerate_hz) {
    T w0 = 2.0 * M_PI * center_freq / samplerate_hz;
    T alpha = sin(w0) / (2.0 * Q);

    T b0 = Q * alpha;
    T b1 = 0;
    T b2 = -Q * alpha;
    T a1 = 2.0 * cos(w0) * (1.0 - w0*w0) / (1.0 + alpha);
    T a2 = (1.0 - alpha) / (1.0 + alpha);

    return IIRFilterCoefficients<T>{1.0, b0, b1, b2, a1, a2};
}

template <FloatingPoint T>
IIRFilterCoefficients<T> SecondOrderButterworthResonantNotch(T center_freq, T Q, T samplerate_hz) {
    T w0 = 2.0 * M_PI * center_freq / samplerate_hz;
    T alpha = sin(w0) / (2.0 * Q);
    T cos_w0 = cos(w0);

    T b0 = 1.0;
    T b1 = -2.0 * cos_w0;
    T b2 = 1.0;
    T a1 = -2.0 * cos_w0 * (1.0 - w0*w0) / (1.0 + alpha);
    T a2 = (1.0 - alpha) / (1.0 + alpha);

    return IIRFilterCoefficients<T>{1.0, b0, b1, b2, a1, a2};
}

template <FloatingPoint T>
IIRFilterCoefficients<T> SecondOrderButterworthResonantHighShelf(T center_freq, T Q, T gain_db, T samplerate_hz) {
    T w0 = 2.0 * M_PI * center_freq / samplerate_hz;
    T alpha = sin(w0) / (2.0 * Q);
    T A = pow(10.0, gain_db / 40.0);
    T cos_w0 = cos(w0);
    T sqrt_A = sqrt(A);

    T b0 = A * ((A + 1.0) + (A - 1.0) * cos_w0 + 2.0 * sqrt_A * alpha);
    T b1 = -2.0 * A * ((A - 1.0) + (A + 1.0) * cos_w0);
    T b2 = A * ((A + 1.0) + (A - 1.0) * cos_w0 - 2.0 * sqrt_A * alpha);
    T a0 = (A + 1.0) - (A - 1.0) * cos_w0 + 2.0 * sqrt_A * alpha;
    T a1 = 2.0 * ((A - 1.0) - (A + 1.0) * cos_w0);
    T a2 = (A + 1.0) - (A - 1.0) * cos_w0 - 2.0 * sqrt_A * alpha;

    NormalizeIIRFilterCoefficients(a0, b0, b1, b2, a1, a2);

    return IIRFilterCoefficients<T>{a0, b0, b1, b2, a1, a2};
}

template <FloatingPoint T>
IIRFilterCoefficients<T> SecondOrderButterworthResonantPeakingEQ(T center_freq, T Q, T gain_db, T samplerate_hz) {
    T w0 = 2.0 * M_PI * center_freq / samplerate_hz;
    T alpha = sin(w0) / (2.0 * Q);
    T A = pow(10.0, gain_db / 40.0);
    T cos_w0 = cos(w0);

    T b0 = 1.0 + alpha * A;
    T b1 = -2.0 * cos_w0;
    T b2 = 1.0 - alpha * A;
    T a0 = 1.0 + alpha / A;
    T a1 = -2.0 * cos_w0;
    T a2 = 1.0 - alpha / A;

    NormalizeIIRFilterCoefficients(a0, b0, b1, b2, a1, a2);

    return IIRFilterCoefficients<T>{1.0, b0, b1, b2, a1, a2};
}

template <FloatingPoint T>
IIRFilterCoefficients<T> SecondOrderButterworthResonantLowShelf(T center_freq, T Q, T gain_db, T samplerate_hz) {
    T w0 = 2.0 * M_PI * center_freq / samplerate_hz;
    T alpha = sin(w0) / (2.0 * Q);
    T A = pow(10.0, gain_db / 40.0);
    T cos_w0 = cos(w0);
    T sqrt_A = sqrt(A);

    T b0 = A * ((A + 1.0) - (A - 1.0) * cos_w0 + 2.0 * sqrt_A * alpha);
    T b1 = 2.0 * A * ((A - 1.0) - (A + 1.0) * cos_w0);
    T b2 = A * ((A + 1.0) - (A - 1.0) * cos_w0 - 2.0 * sqrt_A * alpha);
    T a0 = (A + 1.0) + (A - 1.0) * cos_w0 + 2.0 * sqrt_A * alpha;
    T a1 = -2.0 * ((A - 1.0) + (A + 1.0) * cos_w0);
    T a2 = (A + 1.0) + (A - 1.0) * cos_w0 - 2.0 * sqrt_A * alpha;

    NormalizeIIRFilterCoefficients(a0, b0, b1, b2, a1, a2);

    return IIRFilterCoefficients<T>{1.0, b0, b1, b2, a1, a2};
}