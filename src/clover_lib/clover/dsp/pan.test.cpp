// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdio>

#include <gtest/gtest.h>

#include "clover/dsp/pan.hpp"

using namespace clover;
using namespace dsp;

TEST(dsp_pan, initializes_center) {
    pan p;

    EXPECT_FLOAT_EQ(p.m_pan_L, std::sqrt(2.f));
    EXPECT_FLOAT_EQ(p.m_pan_R, std::sqrt(2.f));
}

TEST(dsp_pan, pans_left_and_right) {
    pan p;

    p.set(-1);
    EXPECT_FLOAT_EQ(p.m_pan_L, 1);
    EXPECT_NEAR(p.m_pan_R, 0, 1e-08);

    p.set(1);
    EXPECT_NEAR(p.m_pan_L, 0, 4.372e-08);  // annoying.
    EXPECT_FLOAT_EQ(p.m_pan_R, 1);
}
