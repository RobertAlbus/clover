#include "clover/io/sample_clock.hpp"

namespace clover::io {

sample_clock::sample_clock() {
    m_current_sample = 0;
}

int_fast64_t sample_clock::current_sample() { return m_current_sample; }

void sample_clock::tick() {
  ++m_current_sample;
}

}