#pragma once

#include <cmath>
#include <concepts>
#include <numbers>
#include <type_traits>

namespace copal::stdlib {

template<std::floating_point T>
T fabs(T x) {
    if constexpr (std::is_same_v<T, float>) {
        return std::fabsf(x);
    } else if constexpr (std::is_same_v<T, double>) {
        return std::fabs(x);
    } else if constexpr (std::is_same_v<T, long double>) {
        return std::fabsl(x);
    } else {
        static_assert(std::is_floating_point_v<T>, "copal::stdlib::fabs requires a floating point type.");
    }
}

template<std::floating_point T>
T fmod(T a, T b) {
    if constexpr (std::is_same_v<T, float>) {
        return std::fmodf(a, b);
    } else if constexpr (std::is_same_v<T, double>) {
        return std::fmod(a, b);
    } else if constexpr (std::is_same_v<T, long double>) {
        return std::fmodl(a, b);
    } else {
        static_assert(std::is_floating_point_v<T>, "copal::stdlib::fmod requires a floating point type.");
    }
}

template<std::floating_point T>
T lerp(T a, T b, T lerp) {
    return std::lerp<T>(a, b, lerp);
}

template<std::floating_point T>
T floor(T x) {
    if constexpr (std::is_same_v<T, float>) {
        return std::floorf(x);
    } else if constexpr (std::is_same_v<T, double>) {
        return std::floor(x);
    } else if constexpr (std::is_same_v<T, long double>) {
        return std::floorl(x);
    } else {
        static_assert(std::is_floating_point_v<T>, "copal::stdlib::floor requires a floating point type.");
    }
}

template<std::floating_point T>
void angle_normalization_pi_over_2(T& x, T& sign_out) {
    constexpr T halfPi = std::numbers::pi_v<T> * T(0.5);
    constexpr T pi = std::numbers::pi_v<T>;
    constexpr T twoPi = std::numbers::pi_v<T> * T(2);

    sign_out = 1;
    if (x > twoPi || x < -twoPi) {
          x = fmod(x, twoPi);
    }

    if (x < 0) x += twoPi;
    if (x > pi) {
        x -= pi;
        sign_out = T(-1);
    }
    if (x > halfPi) x = pi - x;
}

}