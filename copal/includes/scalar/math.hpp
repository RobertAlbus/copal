#pragma once

#include <cmath>
#include <concepts>
#include <numbers>
#include <type_traits>

namespace copal::scalar {

template<std::floating_point T>
T fabs(T x) {
  return x < T(0) ? -x : x;
}

template<std::floating_point T>
T fmod(T a, T b) {
  T sign = 1;
  b = fabs<T>(b);
  if (a < 0) {
    sign = -1;
    a = fabs<T>(a);
  }

  if (a <= b) return a;

  T fitTimes = a / b;
  T remainder = fitTimes - floor(fitTimes);

  return remainder * b * sign;
}

template<std::floating_point T>
T lerp(T a, T b, T lerp) {
  return (T(1) - lerp) * a + lerp * b;
}

template<std::floating_point T>
T floor(T x) {
  if constexpr (std::is_same_v<T, float>) {
    return T(static_cast<int>(x));
  } else if constexpr (std::is_same_v<T, double>) {
    return T(static_cast<long int>(x));
  } else {
    static_assert(false, "copal::scalar::floor requires a float or double.");
  }
}

template<std::floating_point T>
std::pair<T, T>  angle_normalization_pi_over_2(T x) {
  constexpr T halfPi = std::numbers::pi_v<T> * T(0.5);
  constexpr T pi = std::numbers::pi_v<T>;
  constexpr T twoPi = std::numbers::pi_v<T> * T(2);

  T sign = 1;
  if (x > twoPi || x < -twoPi) {
    x = fmod(x, twoPi);
  }

  if (x < 0) x += twoPi;
  if (x > pi) {
    x -= pi;
    sign = -1;
  }
  if (x > halfPi) x = pi - x;
  
  return { x, sign };
}

}