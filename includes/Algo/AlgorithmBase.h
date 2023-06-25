#pragma once

template <typename T_in, typename T_out> struct AlgorithmBase {
  AlgorithmBase() : processed() {}

  virtual T_out process(const T_in &input) = 0;

  const T_out &last() const { return processed; }

protected:
  T_out processed;
};
