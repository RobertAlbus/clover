#include <gtest/gtest.h>

#include "Clover.h"

TEST(Util_CircularBuffer, ShouldBufferCircularly) {
  CircularBuffer<float> buff(3);

  buff.Tick(1.);
  EXPECT_EQ(buff.Read(), 0);

  buff.Tick(2.);
  EXPECT_EQ(buff.Read(), 0);

  buff.Tick(3.);
  EXPECT_EQ(buff.Read(), 1.);

  buff.Tick(4.);
  EXPECT_EQ(buff.Read(), 2.);
  EXPECT_EQ(buff.Read(), 2.);

}