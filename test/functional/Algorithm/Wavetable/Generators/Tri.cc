/*
 * /////////
 * // Clover
 *
 * Audio processing algorithms and DAG with feedback loops that do not break
 * acyclicity.
 *
 * Copyright (C) 2023 Rob W. Albus
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version. This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <gtest/gtest.h>

#include <vector>

#include "Clover/Algorithm/Wavetable/Generators.h"

TEST(Algorithm_Wavetable_Generator_Tri, ShouldBeTriangle) {
  std::vector<float> table5 = Clover::Wavetable::Tri<float>(5);

  EXPECT_FLOAT_EQ(table5[0], 0.f);
  EXPECT_FLOAT_EQ(table5[1], 1.f);
  EXPECT_FLOAT_EQ(table5[2], 0.f);
  EXPECT_FLOAT_EQ(table5[3], -1.f);
  EXPECT_FLOAT_EQ(table5[4], 0.f);

  std::vector<float> table103 = Clover::Wavetable::Tri<float>(103);

  EXPECT_FLOAT_EQ(table103[0], 0.f);
  EXPECT_FLOAT_EQ(table103[25], 1.f);
  EXPECT_FLOAT_EQ(table103[51], 0.f);
  EXPECT_FLOAT_EQ(table103[76], -1.f);
  EXPECT_FLOAT_EQ(table103[102], 0.f);
}

TEST(Algorithm_Wavetable_Generator_Tri, ShouldBeNormalized) {
  int size = 88;
  std::vector<float> table = Clover::Wavetable::Tri<float>(size);

  float absMaxima = 0.f;
  for (int i = 0; i < size; i++) {
    float abs = fabsf(table[i]);
    if (abs > absMaxima)
      absMaxima = abs;
  }

  EXPECT_FLOAT_EQ(absMaxima, 1.f);
}
