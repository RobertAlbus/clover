#include <cmath>

#include "IIRCoefficientStrategy.h"
#include "IIR_Coefficients.h"
#include "Util.h"

namespace Clover::Filter {

template <FloatingPoint T>
struct ResonantButterworthCoefficientStrategy
    : public IIRCoefficientStrategy<T> {
  IIRFilterCoefficients<T> lowPass(T cutoff_hz, T Q, T samplerate_hz) {
    T w0 = 2.0 * M_PI * cutoff_hz / samplerate_hz;
    T alpha = sin(w0) / (2.0 * Q);

    T b0 = w0 * w0 / (1.0 + 2.0 * alpha + w0 * w0);
    T b1 = 2.0 * b0;
    T b2 = b0;
    T a0 = T(1);
    T a1 = 2.0 * (w0 * w0 - 1.0) / (1.0 + 2.0 * alpha + w0 * w0);
    T a2 = (1.0 - 2.0 * alpha + w0 * w0) / (1.0 + 2.0 * alpha + w0 * w0);

    IIRFilterCoefficients<T> coefficients = {b0, b1, b2, a0, a1, a2};
    coefficients.normalize();

    return coefficients;
  }

  IIRFilterCoefficients<T> highPass(T cutoff_hz, T Q, T samplerate_hz) {
    T w0 = 2.0 * M_PI * cutoff_hz / samplerate_hz;
    T alpha = sin(w0) / (2.0 * Q);

    T b0 = (1.0 + cos(w0)) / 2.0;
    T b1 = -(1.0 + cos(w0));
    T b2 = b0;
    T a0 = T(1);
    T a1 = 2.0 * (1.0 - w0 * w0) / (1.0 + 2.0 * alpha + w0 * w0);
    T a2 = (1.0 - 2.0 * alpha + w0 * w0) / (1.0 + 2.0 * alpha + w0 * w0);

    IIRFilterCoefficients<T> coefficients = {b0, b1, b2, a0, a1, a2};
    coefficients.normalize();

    return coefficients;
  }

  IIRFilterCoefficients<T> bandPass(T cutoff_hz, T Q, T samplerate_hz) {
    T w0 = 2.0 * M_PI * cutoff_hz / samplerate_hz;
    T alpha = sin(w0) / (2.0 * Q);

    T b0 = Q * alpha;
    T b1 = 0;
    T b2 = -Q * alpha;
    T a0 = T(1);
    T a1 = 2.0 * cos(w0) * (1.0 - w0 * w0) / (1.0 + alpha);
    T a2 = (1.0 - alpha) / (1.0 + alpha);

    IIRFilterCoefficients<T> coefficients = {b0, b1, b2, a0, a1, a2};
    coefficients.normalize();

    return coefficients;
  }

  IIRFilterCoefficients<T> notch(T cutoff_hz, T Q, T samplerate_hz) {
    T w0 = 2.0 * M_PI * cutoff_hz / samplerate_hz;
    T alpha = sin(w0) / (2.0 * Q);
    T cos_w0 = cos(w0);

    T b0 = 1.0;
    T b1 = -2.0 * cos_w0;
    T b2 = 1.0;
    T a0 = T(1);
    T a1 = -2.0 * cos_w0 * (1.0 - w0 * w0) / (1.0 + alpha);
    T a2 = (1.0 - alpha) / (1.0 + alpha);

    IIRFilterCoefficients<T> coefficients = {b0, b1, b2, a0, a1, a2};
    coefficients.normalize();

    return coefficients;
  }

  IIRFilterCoefficients<T> lowShelf(T cutoff_hz, T Q, T gain_db,
                                    T samplerate_hz) {
    T w0 = 2.0 * M_PI * cutoff_hz / samplerate_hz;
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

    IIRFilterCoefficients<T> coefficients = {b0, b1, b2, a0, a1, a2};
    coefficients.normalize();

    return coefficients;
  }

  IIRFilterCoefficients<T> highShelf(T cutoff_hz, T Q, T gain_db,
                                     T samplerate_hz) {
    T w0 = 2.0 * M_PI * cutoff_hz / samplerate_hz;
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

    IIRFilterCoefficients<T> coefficients = {b0, b1, b2, a0, a1, a2};
    coefficients.normalize();

    return coefficients;
  }

  IIRFilterCoefficients<T> peakingEQ(T cutoff_hz, T Q, T gain_db,
                                     T samplerate_hz) {
    T w0 = 2.0 * M_PI * cutoff_hz / samplerate_hz;
    T alpha = sin(w0) / (2.0 * Q);
    T A = pow(10.0, gain_db / 40.0);
    T cos_w0 = cos(w0);

    T b0 = 1.0 + alpha * A;
    T b1 = -2.0 * cos_w0;
    T b2 = 1.0 - alpha * A;
    T a0 = 1.0 + alpha / A;
    T a1 = -2.0 * cos_w0;
    T a2 = 1.0 - alpha / A;

    IIRFilterCoefficients<T> coefficients = {b0, b1, b2, a0, a1, a2};
    coefficients.normalize();

    return coefficients;
  }
};

} // namespace Clover::Filter
