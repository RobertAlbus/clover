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

#include "Algo/DynamicRange/UpwardDCCompression.h"

TEST(Algorithm_Dynamic_Range_BipolarDCOffest, ShouldCompressUpwards) {
  Clover::DynamicRange::UpwardDCCompression<float, 1> avoider(0.1f);

  std::array<float, 1> value1{0.05f};
  std::array<float, 1> value2{0.5f};
  std::array<float, 1> value3{0.8f};

  EXPECT_GT(avoider.process(value1), value1);
  EXPECT_GT(avoider.process(value2), value2);
  EXPECT_GT(avoider.process(value3), value3);

  value1[0] = value1[0] * -1.f;
  value2[0] = value2[0] * -1.f;
  value3[0] = value3[0] * -1.f;

  EXPECT_LT(avoider.process(value1), value1);
  EXPECT_LT(avoider.process(value2), value2);
  EXPECT_LT(avoider.process(value3), value3);
}
