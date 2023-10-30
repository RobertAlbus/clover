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

#include <chrono>
#include <iostream>

int main(int argc, char *argv[]) {
  const int iterations = 1000000;

    // Measure float multiplication
    auto start = std::chrono::high_resolution_clock::now();
    float f = 1.0f;
    for (int i = 0; i < iterations; ++i) {
        f *= 1.000001f;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto float_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time taken for float multiplication: " << float_duration << " microseconds\n";

    // Measure double multiplication
    start = std::chrono::high_resolution_clock::now();
    double d = 1.0;
    for (int i = 0; i < iterations; ++i) {
        d *= 1.000001f;
    }
    end = std::chrono::high_resolution_clock::now();
    auto double_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time taken for double multiplication: " << double_duration << " microseconds\n";

    // Measure long double multiplication
    start = std::chrono::high_resolution_clock::now();
    long double ld = 1.0L;
    for (int i = 0; i < iterations; ++i) {
        ld *= 1.000001f;
    }
    end = std::chrono::high_resolution_clock::now();
    auto long_double_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time taken for long double multiplication: " << long_double_duration << " microseconds\n";

    return 0;
  
}
