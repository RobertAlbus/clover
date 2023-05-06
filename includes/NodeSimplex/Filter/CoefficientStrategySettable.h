#pragma once

#include <array>
#include <cmath>
#include <functional>

#include "Algorithm.h"
#include "Base.h"
#include "Graph.h"

namespace Clover::NodeSimplex::Filter {

class CoefficientStrategySettable {
public:
  virtual void butterworthResonant() = 0;

  virtual void butterworth() = 0;

  virtual void chebyshevType1() = 0;

  virtual void chebyshevType2() = 0;
};

} // namespace Clover::NodeSimplex::Filter
