#pragma once

#include <math.h>

#include "NodeSimplex.h"

namespace Clover::NodeSimplex::Basic {

template <size_t __arity>
auto rectifierFn = [](Frame<__arity> input) {
  Frame<__arity> f;
  for (int i = 0; i < __arity; i++) {
    f[i] = fabs(input[i]);
  }

  return f;
};
template <size_t __arity>
class Rectifier
    : public StatelessProcessor<__arity, __arity, rectifierFn<__arity>> {};

} // namespace Clover::NodeSimplex::Basic