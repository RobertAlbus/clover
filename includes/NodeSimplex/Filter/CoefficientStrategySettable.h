#pragma once

namespace Clover::NodeSimplex::Filter {

class CoefficientStrategySettable {
public:
  virtual void butterworthResonant() = 0;

  virtual void butterworth() = 0;

  virtual void chebyshevType1() = 0;

  virtual void chebyshevType2() = 0;
};

} // namespace Clover::NodeSimplex::Filter
