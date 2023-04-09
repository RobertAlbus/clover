// DIRECT FORM 1

#pragma once

#include <array>
#include <cmath>
#include <functional>

#include "Constants.h"
#include "Graph.h"

namespace Clover::Algo::Filter {

const float RECIP_SQRT_2 = static_cast<float>(M_SQRT1_2);
const float TWO_PI = M_PI * 2.0f;

class StolenBiquad {
public:
  // implements DF1 topology
  //! Default constructor creates a second-order pass-through filter.
  StolenBiquad(float sampleRate) : inputs_{}, outputs_{}, sampleRate_(sampleRate) {
    float cutoff = sampleRate * 0.45f;
    float reso = RECIP_SQRT_2;
    setLowPass(cutoff, reso);
  }

  float tick(float input) {
    inputs_[0] = input;

    // clang-format off
    lastSample_ = (b_[0] / a_[0]) * inputs_[0] 
                + (b_[1] / a_[0]) * inputs_[1] 
                + (b_[2] / a_[0]) * inputs_[2]
                - (a_[2] / a_[0]) * outputs_[2]
                - (a_[1] / a_[0]) * outputs_[1];
                // reformat this into the co-eff calculation
    // clang-format on

    inputs_[2] = inputs_[1];
    inputs_[1] = inputs_[0];
    outputs_[2] = outputs_[1];
    outputs_[1] = lastSample_;

    return lastSample_;
  }

  void setLowPass(float freq, float reso) {
    setCommonFilterValues(freq, reso, 0);
    float cos_w0 = cosf(w0_);

    b_[0] = (1 - cos_w0) / 2;
    b_[1] = 1.f - cos_w0;
    b_[2] = b_[0];
    a_[0] = 1.f + alpha_;
    a_[1] = -2.f * cos_w0;
    a_[2] = 1.f - alpha_;
  }

  void setHighPass(float freq, float reso = RECIP_SQRT_2) {
    setCommonFilterValues(freq, reso, 0);
    float cos_w0 = cosf(w0_);

    b_[0] = (1 + cos_w0) / 2;
    b_[1] = -(1.f - cos_w0);
    b_[2] = b_[0];
    a_[0] = 1.f + alpha_;
    a_[1] = -2.f * cos_w0;
    a_[2] = 1.f - alpha_;
  }

  void setBandPass(float freq, float reso) {
    setCommonFilterValues(freq, reso, 0);
    float cos_w0 = cosf(w0_);

    b_[0] = sinf(w0_) / 2.0f;
    b_[1] = 0.f;
    b_[2] = -b_[0];
    a_[0] = 1.f + alpha_;
    a_[1] = -2.f * cos_w0;
    a_[2] = 1.f - alpha_;
  }

  void setBandPassConstantGain(float freq, float reso) {
    setCommonFilterValues(freq, reso, 0);
    float cos_w0 = cosf(w0_);

    b_[0] = alpha_;
    b_[1] = 0.f;
    b_[2] = -alpha_;
    a_[0] = 1 + alpha_;
    a_[1] = -2.f * cos_w0;
    a_[2] = 1 - alpha_;
  }

  void setBandReject(float freq, float reso) {
    setCommonFilterValues(freq, reso, 0);
    float cos_w0 = cosf(w0_);

    b_[0] = 1;
    b_[1] = -2.f * cos_w0;
    b_[2] = 1;
    a_[0] = 1.f + alpha_;
    a_[1] = -2.f * cos_w0;
    a_[2] = 1.f - alpha_;
  }

  void setAllPass(float freq, float reso) {
    setCommonFilterValues(freq, reso, 0);
    float cos_w0 = cosf(w0_);

    b_[0] = 1.f - alpha_;
    b_[1] = -2.f * cos_w0;
    b_[2] = 1.f + alpha_;
    a_[0] = b_[2];
    a_[1] = b_[1];
    a_[2] = b_[0];
  }

  void setPeakingEQ(float freq, float reso, float dbGain) {
    setCommonFilterValues(freq, reso, dbGain);
    float cos_w0 = cosf(w0_);

    b_[0] = 1.f + alpha_ * A_;
    b_[1] = -2.f * cos_w0;
    b_[2] = 1.f - alpha_ * A_;
    a_[0] = 1.f + alpha_ / A_;
    a_[1] = b_[1];
    a_[2] = 1.f - alpha_ / A_;
  }

  void setLowShelf(float freq, float reso, float dbGain) {
    setCommonFilterValues(freq, reso, dbGain);
    float cos_w0 = cosf(w0_);
    float h = 2.f * sqrtf(A_) * alpha_; // name is not significant

    b_[0] = A_ * ((A_ + 1.f) - (A_ - 1.f) * cos_w0 + h);
    b_[1] = 2.f * A_ * ((A_ - 1.f) - (A_ + 1.f) * cos_w0);
    b_[2] = A_ * ((A_ + 1.f) - (A_ - 1.f) * cos_w0 - h);
    a_[0] = (A_ + 1.f) + (A_ - 1.f) * cos_w0 + h;
    a_[1] = -2.f * ((A_ - 1.f) + (A_ + 1.f) * cos_w0);
    a_[2] = (A_ + 1.f) + (A_ - 1.f) * cos_w0 - h;
  }

  void setHighShelf(float freq, float reso, float dbGain) {
    setCommonFilterValues(freq, reso, dbGain);
    float cos_w0 = cosf(w0_);
    float h = 2.f * sqrtf(A_) * alpha_; // name is not significant

    b_[0] = A_ * ((A_ + 1.f) + (A_ - 1.f) * cos_w0 + h);
    b_[1] = -2.f * A_ * ((A_ - 1.f) + (A_ + 1.f) * cos_w0);
    b_[2] = A_ * ((A_ + 1.f) + (A_ - 1.f) * cos_w0 - h);
    a_[0] = (A_ + 1.f) - (A_ - 1.f) * cos_w0 + h;
    a_[1] = 2.f * ((A_ - 1.f) - (A_ + 1.f) * cos_w0);
    a_[2] = (A_ + 1.f) - (A_ - 1.f) * cos_w0 - h;
  }

protected:
  float freq_;
  float reso_;
  float dbGain_;

  float w0_;    // angular velocity
  float alpha_; // angular acceleration
  float sampleRate_;
  float A_; // amplitude, peaking and shelving only

  float lastSample_;

  std::array<float, 3> b_;
  std::array<float, 3> a_;
  std::array<float, 3> outputs_;
  std::array<float, 3> inputs_;

  void clear() {
    for (int i = 0; i < 3; i++) {
      outputs_[i] = 0;
      inputs_[i] = 0;
      b_[i] = 0;
      a_[i] = 0;
    }
  }

  void setCommonFilterValues(float freq, float reso, float dbGain) {
    freq_ = freq;
    reso_ = reso;
    dbGain_ = dbGain;

    A_ = powf(10, dbGain / 40.);
    w0_ = 2 * PI * (freq_ / sampleRate_);
    alpha_ = sinf(w0_) / 2.f * reso;
  }
};

} // namespace Clover::Algo::Filter
