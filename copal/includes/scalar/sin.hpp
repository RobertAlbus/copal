#pragma once

#include <concepts>
#include <experimental/bits/simd.h>
#include <type_traits>

#include "math.hpp"
#include "sin_lut.hpp"

namespace copal::scalar {

template<std::floating_point T>
T sin_taylor(T x) {
  T sign = T(1);
  angle_normalization_pi_over_2(x, sign);
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
T sin_lookup(T x) {
  T sign = T(1);
  angle_normalization_pi_over_2(x, sign);

  T index_f = x * (lut::max_index);
  T index_A = static_cast<size_t>(index_f);

  // if constexpr (std::is_same_v<T, float>) {
  //     // do not use lerp for single precision float.
  //     // precision in table size 2048 for [0..0.25 pi] 
  //     // is same as rounding error
  //     return sign * copal::lut::get<T>()[index_A];
  // }

  // TODO: bounds checking for (A < maxIndex - 1)
  T index_B = index_A + T(1);
  T lerpAmount = index_f - index_A;

  return sign * lerp(
      copal::lut::get<T>()[index_A],
      copal::lut::get<T>()[index_B],
      lerpAmount
  );
}

}
