#pragma once
#include <vector>

template <typename T> class HistoryBuffer {
public:
  HistoryBuffer(int size)
      : buffer_(size, 0.), index_(0) {}

  void tick(T value) {
    buffer_[(++index_ % buffer_.size())] = value;
  }

  T read(int atIndex = 0) {
    // should it error if atIndex is greater than size?
    return buffer_[(index_ - atIndex) % buffer_.size()];
  }

  int size() const { return buffer_.size(); }

private:
  std::vector<T> buffer_;
  int index_;
};
