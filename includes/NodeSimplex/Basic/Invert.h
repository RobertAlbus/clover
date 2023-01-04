#pragma once

#include "NodeSimplex.h"

namespace Clover::NodeSimplex::Basic {

template <size_t __arity>
auto invertFn = [](Frame<__arity> input) {
  Frame<__arity> f;
  for (int i = 0; i < __arity; i++) {
    f[i] = input[i] * -1.;
  }

  return f;
};
template <size_t __arity>
class Invert : public StatelessProcessor<__arity, __arity, invertFn<__arity>> {
};

} // namespace Clover::NodeSimplex::Basic