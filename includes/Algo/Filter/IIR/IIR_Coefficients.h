#pragma once

#include "Util.h"

template <FloatingPoint T> struct IIRFilterCoefficients {
  T b0;
  T b1;
  T b2;
  T a0;
  T a1;
  T a2;

  void normalize() {
    T a0_inv = T(1) / a0;
    b0 *= a0_inv;
    b1 *= a0_inv;
    b2 *= a0_inv;
    a0 = T(1);
    a1 *= a0_inv;
    a2 *= a0_inv;
  }
};
