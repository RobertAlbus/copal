#pragma once

#include <cmath>
#include <concepts>
#include <type_traits>

namespace copal::stdlib {

template<std::floating_point T>
T sin_stdlib(T x) {
    if constexpr (std::is_same_v<T, float>) {
        return std::sinf(x);
    } else if constexpr (std::is_same_v<T, double>) {
        return std::sin(x);
    } else if constexpr (std::is_same_v<T, long double>) {
        return std::sinl(x);
    } else {
        static_assert(std::is_floating_point_v<T>, "std_sin requires a floating point type.");
    }
}

}
