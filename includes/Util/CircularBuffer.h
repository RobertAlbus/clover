#pragma once
#include <vector>

template <typename T> class CircularBuffer {
public:
  CircularBuffer(int size)
      : buffer_(size), next_read_index_(0), next_write_index_(size - 1),
        current_read_index_(0) {}

  T Tick(T value) {
    current_read_index_ = next_read_index_;
    buffer_[next_write_index_] = value;
    next_read_index_ = (next_read_index_ + 1) % buffer_.size();
    next_write_index_ = (next_write_index_ + 1) % buffer_.size();
    return buffer_[current_read_index_];
  }

  T Read() { return buffer_[current_read_index_]; }

private:
  std::vector<T> buffer_;
  int next_read_index_;
  int next_write_index_;
  int current_read_index_;
};
