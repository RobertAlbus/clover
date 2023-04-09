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
