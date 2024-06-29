#pragma once

#include <cmath>
#include <concepts>
#include <numbers>
#include <type_traits>
#include <utility>

#include "num.hpp"

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
    a *= -1;
  }

  if (a == b) return 0;
  if (a <= b) return a;

  T fitTimes = a / b;
  T remainder = fitTimes - floor(fitTimes);

  return remainder * b * sign;
}

template<std::floating_point T>
T lerp(T a, T b, T lerp) {
  if (std::isinf(a) || std::isinf(b) || std::isinf(lerp))
    return a*b*lerp;
  return (T(1) - lerp) * a + lerp * b;
}

template<std::floating_point T>
T floor(T x) {
  T fractionalPart = fmod(x, {1});
  T integerPart    = x - fractionalPart;
  if (x < 0 && x != integerPart) {
      return integerPart - 1;
  }
  return integerPart;
}

template<std::floating_point T>
std::pair<T, T>  angle_normalization_pi_over_2(T x) {
  constexpr T halfPi = copal::num::pi_over_2<T>;
  constexpr T pi     = copal::num::pi_x_1<T>;
  constexpr T twoPi  = copal::num::pi_x_2<T>;

  T sign = 1;
  if (x >= twoPi || x <= -twoPi) {
    x = fmod(x, twoPi);
  }

  if (x < 0) x += twoPi;
  if (x >= pi) {
    x -= pi;
    sign = -1;
  }
  if (x > halfPi) x = pi - x;

  return { x, sign };
}

}