#pragma once

#include <concepts>
#include <numbers>
#include <type_traits>

namespace copal::num {

#define pi_over(denom)          \
template<std::floating_point T> \
inline constexpr T pi_over_##denom = std::numbers::pi_v<T> / denom;

#define pi_x(factor)            \
template<std::floating_point T> \
inline constexpr T pi_x_##factor = std::numbers::pi_v<T> * factor;

pi_over(16)
pi_over(8)
pi_over(4)
pi_over(2)
pi_over(1)

pi_x(1)
pi_x(2)
pi_x(4)
pi_x(8)
pi_x(16)

}
