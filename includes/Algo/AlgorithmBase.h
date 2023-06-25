#pragma once

template <typename T> struct AlgorithmBase {
  AlgorithmBase() : processed() {}

  const T &last() const { return processed; }

protected:
  T processed;
};
