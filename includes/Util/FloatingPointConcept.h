#pragma once

template <typename T>
concept FloatingPoint = std::is_floating_point_v<T>;
