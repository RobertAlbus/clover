// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include <gtest/gtest.h>

#include "clover/float.hpp"

using namespace clover;

TEST(float, float_eq) {
    std::vector<float> magnitudes{1.e-38, 1.e-1, 1.e1, 1.e38};
    for (auto magnitude : magnitudes) {
        EXPECT_TRUE(float_eq(std::nextafter(magnitude, 0.0f), magnitude));
        EXPECT_TRUE(float_eq(std::nextafter(-magnitude, 0.0f), -magnitude));
    }
}