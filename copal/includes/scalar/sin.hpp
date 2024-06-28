#pragma once

#include <concepts>
#include <experimental/bits/simd.h>
#include <type_traits>

#include "math.hpp"
#include "sin_lut.hpp"

namespace copal::scalar {

template<std::floating_point T>
T sin_taylor(T xIn) {
  auto [x, sign] = angle_normalization_pi_over_2(xIn);
  T term = x;
  T sum = term;

  // 5 terms for accuracy within 10^-7 for {0 < x < 0.25 pi} 
  for (uint m = 2; m <= 10; m += 2) {
    term *= -x * x / (m * (m + 1));
    sum += term;
  }

  return sign * sum;
}

template<std::floating_point T>
T sin_lookup(T xIn) {
  auto [x, sign] = angle_normalization_pi_over_2(xIn);

  T index_f = x * (lut::max_index);
  size_t index_A = static_cast<size_t>(index_f);

  if (index_A > copal::lut::max_index) {
    return copal::lut::get<T>()[index_A];
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
