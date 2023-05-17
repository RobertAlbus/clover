#pragma once

#include <array>
#include <cmath>
#include <functional>

#include "Algorithm.h"
#include "Base.h"
#include "Graph.h"

namespace Clover::NodeSimplex::Filter {

class FilterSettable {
public:
  virtual void set(float f, float Q) = 0;

  virtual void freq(float f) = 0;
  virtual float freq() = 0;

  virtual void reso(float Q) = 0;
  virtual float reso() = 0;

  virtual void lowPass() = 0;

  virtual void highPass() = 0;

  virtual void notch() = 0;

  virtual void bandPass() = 0;
};

} // namespace Clover::NodeSimplex::Filter
