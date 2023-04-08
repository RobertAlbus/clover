#include <cmath>
#include <vector>

#include "Util.h"

template <FloatingPoint T>
std::vector<T> kaiser_lpf_coefficients(T cutoff_hz, T transition_bw_hz, T sample_rate_hz, T stopband_atten_db) {

  // Compute the filter coefficients using the windowed sinc function
  std::vector<T> coeffs(num_taps);
  T norm_cutoff_freq = static_cast<T>(2.0) * static_cast<T>(M_PI) * cutoff_hz / sample_rate_hz;
  T sum = static_cast<T>(0);
  for (int n = 0; n < num_taps; ++n) {
    T x = static_cast<T>(n) - mid_point;
    coeffs[n] = (x == 0) ? window[n] * norm_cutoff_freq / static_cast<T>(M_PI)
                         : window[n] * std::sin(norm_cutoff_freq * x) / (static_cast<T>(M_PI) * x);

    sum += coeffs[n];
  }

  // Normalize the filter coefficients to achieve unity gain at DC
  for (int n = 0; n < num_taps; ++n) {
    coeffs[n] /= sum;
  }

  return coeffs;
}

template <FloatingPoint T>
std::vector<T> kaiser_hpf_coefficients(T cutoff_hz, T transition_bw_hz, T sample_rate_hz, T stopband_atten_db) {
  auto coeffs = kaiser_lpf_coefficients<T>(cutoff_hz, transition_bw_hz, sample_rate_hz, stopband_atten_db);

  int num_taps = coeffs.size();
  T mid_point = static_cast<T>(num_taps - 1) / static_cast<T>(2.0);

  for (int n = 0; n < num_taps; ++n) {
    coeffs[n] = -coeffs[n];
    if (n == mid_point) {
      coeffs[n] += static_cast<T>(1);
    }
  }

  return coeffs;
}

template <FloatingPoint T>
std::vector<T> kaiser_bpf_coefficients(T low_cutoff_hz, T high_cutoff_hz, T transition_bw_hz, T sample_rate_hz,
                                       T stopband_atten_db) {

  // Design low-pass and high-pass filters
  auto low_pass_coeffs = kaiser_lpf_coefficients<T>(low_cutoff_hz, transition_bw_hz, sample_rate_hz, stopband_atten_db);
  auto high_pass_coeffs =
      kaiser_hpf_coefficients<T>(high_cutoff_hz, transition_bw_hz, sample_rate_hz, stopband_atten_db);

  // Combine low-pass and high-pass filters to create a band-pass filter
  int num_taps = low_pass_coeffs.size();
  std::vector<T> band_pass_coeffs(num_taps);

  for (int n = 0; n < num_taps; ++n) {
    band_pass_coeffs[n] = low_pass_coeffs[n] * high_pass_coeffs[n];
  }

  return band_pass_coeffs;
}

template <FloatingPoint T>
std::vector<T> kaiser_notch_coefficients(T center_freq_hz, T notch_bw_hz, T transition_bw_hz, T sample_rate_hz,
                                         T stopband_atten_db) {
  // Design a bandpass filter
  T low_cutoff_hz = center_freq_hz - notch_bw_hz / static_cast<T>(2.0);
  T high_cutoff_hz = center_freq_hz + notch_bw_hz / static_cast<T>(2.0);
  auto band_pass_coeffs =
      kaiser_bpf_coefficients<T>(low_cutoff_hz, high_cutoff_hz, transition_bw_hz, sample_rate_hz, stopband_atten_db);

  // Calculate the number of taps
  int num_taps = band_pass_coeffs.size();

  // Create a scaled impulse (delta function) with the same length as the
  // bandpass filter coefficients
  std::vector<T> impulse(num_taps, static_cast<T>(0));
  impulse[num_taps / 2] = static_cast<T>(1);

  // Subtract the bandpass coefficients from the impulse to create the notch
  // filter coefficients
  std::vector<T> notch_coeffs(num_taps);
  for (int n = 0; n < num_taps; ++n) {
    notch_coeffs[n] = impulse[n] - band_pass_coeffs[n];
  }

  return notch_coeffs;
}
