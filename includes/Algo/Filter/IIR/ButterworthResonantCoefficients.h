#include <cmath>

#include "IIRCoefficientStrategy.h"
#include "IIR_Coefficients.h"
#include "Util.h"

namespace Clover::Filter {

template <FloatingPoint T>
struct ResonantButterworthCoefficientStrategy
    : public IIRCoefficientStrategy<T> {
  IIRFilterCoefficients<T> lowPass(T cutoff_hz, T Q, T samplerate_hz) {
    T w0 = T(2) * M_PI * cutoff_hz / samplerate_hz;
    T alpha = sin(w0) / (T(2) * Q);

    T b0 = w0 * w0 / (T(1) + T(2) * alpha + w0 * w0);
    T b1 = T(2) * b0;
    T b2 = b0;
    T a0 = T(1);
    T a1 = T(2) * (w0 * w0 - T(1)) / (T(1) + T(2) * alpha + w0 * w0);
    T a2 = (T(1) - T(2) * alpha + w0 * w0) / (T(1) + T(2) * alpha + w0 * w0);

    IIRFilterCoefficients<T> coefficients = {b0, b1, b2, a0, a1, a2};
    coefficients.normalize();

    return coefficients;
  }

  IIRFilterCoefficients<T> highPass(T cutoff_hz, T Q, T samplerate_hz) {
    T w0 = T(2) * M_PI * cutoff_hz / samplerate_hz;
    T alpha = sin(w0) / (T(2) * Q);

    T b0 = (T(1) + cos(w0)) / T(2);
    T b1 = -(T(1) + cos(w0));
    T b2 = b0;
    T a0 = T(1);
    T a1 = T(2) * (T(1) - w0 * w0) / (T(1) + T(2) * alpha + w0 * w0);
    T a2 = (T(1) - T(2) * alpha + w0 * w0) / (T(1) + T(2) * alpha + w0 * w0);

    IIRFilterCoefficients<T> coefficients = {b0, b1, b2, a0, a1, a2};
    coefficients.normalize();

    return coefficients;
  }

  IIRFilterCoefficients<T> bandPass(T cutoff_hz, T Q, T samplerate_hz) {
    T w0 = T(2) * M_PI * cutoff_hz / samplerate_hz;
    T alpha = sin(w0) / (T(2) * Q);

    T b0 = Q * alpha;
    T b1 = T(0);
    T b2 = -Q * alpha;
    T a0 = T(1);
    T a1 = T(2) * cos(w0) * (T(1) - w0 * w0) / (T(1) + alpha);
    T a2 = (T(1) - alpha) / (T(1) + alpha);

    IIRFilterCoefficients<T> coefficients = {b0, b1, b2, a0, a1, a2};
    coefficients.normalize();

    return coefficients;
  }

  IIRFilterCoefficients<T> notch(T cutoff_hz, T Q, T samplerate_hz) {
    T w0 = T(2) * M_PI * cutoff_hz / samplerate_hz;
    T alpha = sin(w0) / (T(2) * Q);
    T cos_w0 = cos(w0);

    T b0 = T(1);
    T b1 = T(-2) * cos_w0;
    T b2 = T(1);
    T a0 = T(1);
    T a1 = T(-2) * cos_w0 * (T(1) - w0 * w0) / (T(1) + alpha);
    T a2 = (T(1) - alpha) / (T(1) + alpha);

    IIRFilterCoefficients<T> coefficients = {b0, b1, b2, a0, a1, a2};
    coefficients.normalize();

    return coefficients;
  }

  IIRFilterCoefficients<T> lowShelf(T cutoff_hz, T Q, T gain_db,
                                    T samplerate_hz) {
    T w0 = T(2) * M_PI * cutoff_hz / samplerate_hz;
    T alpha = sin(w0) / (T(2) * Q);
    T A = pow(T(10), gain_db / T(40));
    T cos_w0 = cos(w0);
    T sqrt_A = sqrt(A);

    T b0 = A * ((A + T(1)) - (A - T(1)) * cos_w0 + T(2) * sqrt_A * alpha);
    T b1 = T(2) * A * ((A - T(1)) - (A + T(1)) * cos_w0);
    T b2 = A * ((A + T(1)) - (A - T(1)) * cos_w0 - T(2) * sqrt_A * alpha);
    T a0 = (A + T(1)) + (A - T(1)) * cos_w0 + T(2) * sqrt_A * alpha;
    T a1 = -T(2) * ((A - T(1)) + (A + T(1)) * cos_w0);
    T a2 = (A + T(1)) + (A - T(1)) * cos_w0 - T(2) * sqrt_A * alpha;

    IIRFilterCoefficients<T> coefficients = {b0, b1, b2, a0, a1, a2};
    coefficients.normalize();

    return coefficients;
  }

  IIRFilterCoefficients<T> highShelf(T cutoff_hz, T Q, T gain_db,
                                     T samplerate_hz) {
    T w0 = T(2) * M_PI * cutoff_hz / samplerate_hz;
    T alpha = sin(w0) / (T(2) * Q);
    T A = pow(T(10), gain_db / T(40));
    T cos_w0 = cos(w0);
    T sqrt_A = sqrt(A);

    T b0 = A * ((A + T(1)) + (A - T(1)) * cos_w0 + T(2) * sqrt_A * alpha);
    T b1 = -T(2) * A * ((A - T(1)) + (A + T(1)) * cos_w0);
    T b2 = A * ((A + T(1)) + (A - T(1)) * cos_w0 - T(2) * sqrt_A * alpha);
    T a0 = (A + T(1)) - (A - T(1)) * cos_w0 + T(2) * sqrt_A * alpha;
    T a1 = T(2) * ((A - T(1)) - (A + T(1)) * cos_w0);
    T a2 = (A + T(1)) - (A - T(1)) * cos_w0 - T(2) * sqrt_A * alpha;

    IIRFilterCoefficients<T> coefficients = {b0, b1, b2, a0, a1, a2};
    coefficients.normalize();

    return coefficients;
  }

  IIRFilterCoefficients<T> peakingEQ(T cutoff_hz, T Q, T gain_db,
                                     T samplerate_hz) {
    T w0 = T(2) * M_PI * cutoff_hz / samplerate_hz;
    T alpha = sin(w0) / (T(2) * Q);
    T A = pow(T(10), gain_db / T(40));
    T cos_w0 = cos(w0);

    T b0 = T(1) + alpha * A;
    T b1 = T(-2) * cos_w0;
    T b2 = T(1) - alpha * A;
    T a0 = T(1) + alpha / A;
    T a1 = T(-2) * cos_w0;
    T a2 = T(1) - alpha / A;

    IIRFilterCoefficients<T> coefficients = {b0, b1, b2, a0, a1, a2};
    coefficients.normalize();

    return coefficients;
  }
};

} // namespace Clover::Filter
