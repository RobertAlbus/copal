#pragma once

#include <array>
#include <cmath>
#include <concepts>
#include <experimental/bits/simd.h>
#include <numbers>
#include <type_traits>

#include "math.hpp"
#include "num.hpp"
#include "sin_lut.hpp"

namespace copal::stdlib {

template<std::floating_point T, size_t terms>
T sin_taylor(T xIn) {
  auto [x, sign] = angle_normalization_pi_over_2(xIn);
  T term = x;
  T sum = term;

  constexpr size_t termsEnd = terms * 2;
  for (size_t m = 2; m <= termsEnd; m += 2) {
    term *= -x * x / (m * (m + 1));
    sum += term;
  }

  return sign * sum;
}

template<std::floating_point T>
T sin_taylor(const T& xInput) {
  return sin_taylor<T, 5>(xInput);
}

template<std::floating_point T>
T sin_lookup(T xIn) {
  auto [x, sign] = angle_normalization_pi_over_2(xIn);
  // T index_f = x * copal::num::pi_over_2<T> * lut::max_index;
  T index_f  = x * (lut::max_index / copal::num::pi_over_2<T>);
  size_t index_A = static_cast<size_t>(index_f);

  if (index_A > copal::lut::max_index) {
    return copal::lut::get<T>()[copal::lut::max_index];
  }

  size_t index_B = index_A + 1;
  T lerpAmount = index_f - index_A;

  return sign * lerp(
    copal::lut::get<T>()[index_A],
    copal::lut::get<T>()[index_B],
    lerpAmount
  );
}

}
