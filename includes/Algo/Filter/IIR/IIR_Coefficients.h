#pragma once

#include "Util.h"

template <FloatingPoint T> struct IIRFilterCoefficients {
  T gain;
  T b0;
  T b1;
  T b2;
  T a1;
  T a2;
};

template <FloatingPoint T>
void NormalizeIIRFilterCoefficients(T &a0, T &b0, T &b1, T &b2, T &a1, T &a2) {
    T a0_inv = 1.0 / a0;
    b0 *= a0_inv;
    b1 *= a0_inv;
    b2 *= a0_inv;
    a1 *= a0_inv;
    a2 *= a0_inv;
    a0 = 1.0;
}