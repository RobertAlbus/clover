// // Clover Audio Framework
// // Copyright (C) 2023  Rob W. Albus
// // Licensed under the GPLv3. See LICENSE for details.

// #include "clover/circular_view.hpp"
// #include <cmath>
// #include <cstdio>

// #include "clover/float.hpp"
// #include <gtest/gtest.h>

// using namespace clover;

// TEST(circular_view, instantiates_at_beginning) {
//     int size   = 10;
//     auto range = std::views::iota(0, size);

//     std::vector<clover_float> data =
//             range | std::views::transform([](int v) { return static_cast<clover_float>(v); }) |
//             std::ranges::to<std::vector>();

//     circular_view circle{data.begin(), data.end()};

//     EXPECT_EQ(circle.m_begin, data.begin());
//     EXPECT_EQ(circle.m_sentinel, data.end());

//     EXPECT_EQ(circle.begin(), data.begin());
// }

// TEST(circular_view, instantiates_from_position) {
//     int size   = 10;
//     auto range = std::views::iota(0, size);

//     std::vector<clover_float> data =
//             range | std::views::transform([](int v) { return static_cast<clover_float>(v); }) |
//             std::ranges::to<std::vector>();

//     auto from = data.begin() + 4;

//     circular_view circle{data.begin(), data.end(), from};

//     EXPECT_EQ(circle.m_begin, data.begin());
//     EXPECT_EQ(circle.m_begin_from, from);
//     EXPECT_EQ(circle.m_sentinel, data.end());

//     EXPECT_EQ(circle.begin().m_begin, data.begin());
//     EXPECT_EQ(circle.begin(), from);
//     EXPECT_EQ(circle.begin().m_sentinel, data.end());
// }

// TEST(circular_view, operator_index) {
//     int size   = 10;
//     auto range = std::views::iota(0, size);

//     std::vector<clover_float> data =
//             range | std::views::transform([](int v) { return static_cast<clover_float>(v); }) |
//             std::ranges::to<std::vector>();

//     circular_view circle{data.begin(), data.end()};

//     for (auto n : range) {
//         EXPECT_FLOAT_EQ(*circle.begin()[n], data[n]);
//     }

//     using difference_type  = decltype(circle)::iterator::difference_type;
//     difference_type offset = 4;
//     auto from              = data.begin() + offset;
//     circular_view circle_offset{data.begin(), data.end(), from};

//     for (auto n : range) {
//         // 0 - 4 --- 4 - 4
//         // 1 - 5 --- 5 - 5
//         // 2 - 6 --- 6 - 6
//         // 3 - 7 --- 7 - 7
//         // 4 - 8 --- 8 - 8
//         // 5 - 9 --- 9 - 9
//         // 6 - 0 --- 4 - 0    <=== ISSUE HERE
//         // 7 - 1 --- 3 - 1
//         // 8 - 2 --- 2 - 2
//         // 9 - 3 --- 1 - 3
//         difference_type i = (n + offset) % (difference_type)size;
//         std::cout << n << " - " << i << " --- " << *circle_offset.begin()[n] << " - " << data[i] <<
//         std::endl;
//     }
//     for (auto n : range) {
//         difference_type i = (n + offset) % (difference_type)size;
//         EXPECT_FLOAT_EQ(*circle_offset.begin()[n], data[i]);
//     }
// }

// TEST(circular_view, for_loop_iterates_once) {
//     int size   = 10;
//     auto range = std::views::iota(0, size);

//     std::vector<clover_float> data =
//             range | std::views::transform([](int v) { return static_cast<clover_float>(v); }) |
//             std::ranges::to<std::vector>();

//     circular_view circle{data.begin(), data.end()};
//     using difference_type = decltype(circle)::iterator::difference_type;

//     difference_type count = 0;
//     for (auto n : circle) {
//         ++count;
//     }
//     EXPECT_FLOAT_EQ(count, circle.size());

//     difference_type offset = 4;
//     auto from              = data.begin() + offset;
//     circular_view circle_offset{data.begin(), data.end(), from};

//     count = 0;
//     for (auto n : circle_offset) {
//         ++count;
//     }
//     EXPECT_FLOAT_EQ(count, circle.size());
// }