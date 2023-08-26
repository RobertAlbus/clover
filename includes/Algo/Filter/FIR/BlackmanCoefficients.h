/*
 * /////////
 * // Clover
 *
 * Audio processing algorithms and DAG with feedback loops that do not break
 * acyclicity.
 *
 * Copyright (C) 2023 Rob W. Albus
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version. This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <cmath>
#include <iostream>
#include <vector>

std::vector<double> blackman_lpf_coefficients(double cutoff_frequency_hz,
                                              double transition_bandwidth_hz,
                                              double sample_rate_hz,
                                              double stopband_attenuation_db) {
  int filter_order = int(ceil((stopband_attenuation_db - 8.0) /
                              (2.285 * (2.0 * M_PI * transition_bandwidth_hz /
                                        sample_rate_hz)))) +
                     1;

  if (filter_order % 2 == 0) {
    filter_order += 1;
  }

  int middle_index = (filter_order - 1) / 2;
  double normalized_cutoff_frequency = cutoff_frequency_hz / sample_rate_hz;
  std::vector<double> coefficients(filter_order, 0.0);

  for (int n = 0; n < filter_order; ++n) {
    double window_multiplier = 0.42 -
                               0.5 * cos(2.0 * M_PI * n / (filter_order - 1)) +
                               0.08 * cos(4.0 * M_PI * n / (filter_order - 1));

    if (n == middle_index) {
      coefficients[n] = 2 * normalized_cutoff_frequency * window_multiplier;
    } else {
      coefficients[n] =
          sin(2 * M_PI * normalized_cutoff_frequency * (n - middle_index)) /
          (M_PI * (n - middle_index)) * window_multiplier;
    }
  }

  return coefficients;
}

std::vector<double> blackman_hpf_coefficients(double cutoff_frequency_hz,
                                              double transition_bandwidth_hz,
                                              double sample_rate_hz,
                                              double stopband_attenuation_db) {
  std::vector<double> lpf_coefficients =
      blackman_lpf_coefficients(cutoff_frequency_hz, transition_bandwidth_hz,
                                sample_rate_hz, stopband_attenuation_db);

  for (size_t i = 0; i < lpf_coefficients.size(); ++i) {
    lpf_coefficients[i] = -lpf_coefficients[i];
    if (i % 2 == 1) {
      lpf_coefficients[i] *= -1;
    }
  }

  return lpf_coefficients;
}

std::vector<double> blackman_bpf_coefficients(double lower_cutoff_frequency_hz,
                                              double upper_cutoff_frequency_hz,
                                              double transition_bandwidth_hz,
                                              double sample_rate_hz,
                                              double stopband_attenuation_db) {
  double center_frequency_hz =
      (upper_cutoff_frequency_hz + lower_cutoff_frequency_hz) / 2.0;
  double bandwidth_hz = upper_cutoff_frequency_hz - lower_cutoff_frequency_hz;

  std::vector<double> coefficients =
      blackman_lpf_coefficients(bandwidth_hz, transition_bandwidth_hz,
                                sample_rate_hz, stopband_attenuation_db);

  for (size_t i = 0; i < coefficients.size(); ++i) {
    coefficients[i] *=
        2 * cos(2 * M_PI * center_frequency_hz * i / sample_rate_hz);
  }

  return coefficients;
}

// does this work?
std::vector<double> blackman_notch_coefficients(
    double notch_frequency_hz, double transition_bandwidth_hz,
    double sample_rate_hz, double stopband_attenuation_db) {
  std::vector<double> lpf_coefficients = blackman_lpf_coefficients(
      transition_bandwidth_hz, transition_bandwidth_hz, sample_rate_hz,
      stopband_attenuation_db);

  for (size_t i = 0; i < lpf_coefficients.size(); ++i) {
    if (i == (lpf_coefficients.size() - 1) / 2) {
      lpf_coefficients[i] = 1 - lpf_coefficients[i];
    } else {
      lpf_coefficients[i] *=
          -2 * cos(2 * M_PI * notch_frequency_hz * i / sample_rate_hz);
    }
  }

  return lpf_coefficients;
}
