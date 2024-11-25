// This file is part of Clover Audio Framework, Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include <gtest/gtest.h>

#include "clover/io/sample_clock.hpp"

using namespace clover;
TEST(sample_clock, monotonically_increasing) {
    io::sample_clock clock;
    EXPECT_EQ(clock.current_sample(), 0);

    clock.tick();
    EXPECT_EQ(clock.current_sample(), 1);

    for (size_t i = 1; i < 10000000; ++i)
        clock.tick();
    
    EXPECT_EQ(clock.current_sample(), 10000000);
}