#pragma once

#include <array>
#include <cmath>
#include <functional>

#include "Algorithm.h"
#include "Base.h"
#include "Graph.h"

#include "CoefficientStrategySettable.h"
#include "FilterSettable.h"

namespace Clover::NodeSimplex::Filter {

class EQSettable {
public:
  void set(float f, float Q, float dbGain);

  void freq(float f);
  float freq();

  void reso(float Q);
  float reso();

  void dbGain(float dbGain);

  void lowShelf();

  void highShelf();

  void peakingEQ();
};

} // namespace Clover::NodeSimplex::Filter
