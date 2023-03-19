#include <gtest/gtest.h>

#include "Clover.h"

TEST(Util_HistoryBuffer, ShouldHaveSize) {
  HistoryBuffer<float> buff(3);

  EXPECT_EQ(buff.size(), 3);
}

TEST(Util_HistoryBuffer, ShouldUpdate) {
  HistoryBuffer<float> buff(3);

  buff.tick(1.);
  EXPECT_EQ(buff.read(), 1.);
  buff.tick(2.);
  EXPECT_EQ(buff.read(), 2.);
  buff.tick(3.);
  EXPECT_EQ(buff.read(), 3.);
}

TEST(Util_HistoryBuffer, ShouldHaveHistory) {
  HistoryBuffer<float> buff(3);

  buff.tick(1.);
  buff.tick(2.);
  buff.tick(3.);
  EXPECT_EQ(buff.read(), 3.);
  EXPECT_EQ(buff.read(1), 2.);
  EXPECT_EQ(buff.read(2), 1.);
}