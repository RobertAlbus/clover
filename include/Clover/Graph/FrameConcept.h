#pragma once

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

#include <concepts>

// clang-format off
template <typename T>
concept Frame = requires(T a, const T b, const T c, const float f) {
                  T();

                  { a.init() } -> std::same_as<void>;

                  { b + c } -> std::same_as<T>;
                  { b - c } -> std::same_as<T>;
                  { b *c } -> std::same_as<T>;
                  { b / c } -> std::same_as<T>;

                  { a += b } -> std::same_as<void>;
                  { a -= b } -> std::same_as<void>;
                  { a *= b } -> std::same_as<void>;
                  { a /= b } -> std::same_as<void>;

                  { a += f } -> std::same_as<void>;
                  { a -= f } -> std::same_as<void>;
                  { a *= f } -> std::same_as<void>;
                  { a /= f } -> std::same_as<void>;
                };
// clang-format on
