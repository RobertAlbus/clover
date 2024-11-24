// This file is part of Clover Audio Framework, Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include <gtest/gtest.h>

#include "clover/example.hpp"

TEST(Example, ExampleTest) { EXPECT_EQ(clover::example(), 0); }