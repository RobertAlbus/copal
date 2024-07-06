#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <experimental/simd>
#include <experimental/bits/simd.h>
#include <experimental/bits/simd_math.h>
#include <type_traits>

#include "math.hpp"
#include "num.hpp"
#include "sin_lut.hpp"
#include "stdlib/math.hpp"
#include "stdx_definition.hpp"

namespace copal::vector_impl {

template<std::floating_point T, size_t terms>
stdx::native_simd<T> sin_taylor(const stdx::native_simd<T>& xInput) {
  auto [x, sign] = angle_normalization_pi_over_2(xInput);
  stdx::native_simd<T> term = x;
  stdx::native_simd<T> sum  = term;
  
  constexpr size_t termsEnd = terms * 2;
  for (size_t m = 2; m <= termsEnd; m += 2) {
    term *= -x * x / T(m * (m + 1));
    sum += term;
  }
  return sum * sign;
}

template<std::floating_point T>
stdx::native_simd<T> sin_taylor(const stdx::native_simd<T>& xInput) {
  return sin_taylor<T, 5>(xInput);
}

template<std::floating_point T>
stdx::native_simd<T> sin_lookup(const stdx::native_simd<T>& xIn) {
  auto [x, sign] = copal::vector_impl::angle_normalization_pi_over_2(xIn);

  using Int = std::conditional_t<std::is_same_v<T, float>, int,
        std::conditional_t<std::is_same_v<T, double>, long int, long long>>;
  stdx::native_simd<T>   index_f  = x / copal::num::pi_over_2<T> * T(lut::max_index);
  stdx::native_simd<Int> index_A  = stdx::static_simd_cast<stdx::native_simd<Int>>(index_f);
  stdx::native_simd<T>   index_Af = stdx::static_simd_cast<stdx::native_simd<T>>(index_A);
  stdx::native_simd<Int> index_B  = ++index_A;

  stdx::native_simd_mask<Int> invalidIndex_i = index_A  >= copal::lut::max_index_i;
  stdx::native_simd_mask<T>   invalidIndex   = index_Af >= copal::lut::max_index_i;
  if (stdx::any_of(invalidIndex_i)) {
    stdx::where(invalidIndex,   index_f ) = T(copal::lut::max_index_i);
    stdx::where(invalidIndex,   index_Af) = T(copal::lut::max_index_i);
    stdx::where(invalidIndex_i, index_A ) = T(copal::lut::max_index_i);
    stdx::where(invalidIndex_i, index_B ) = T(copal::lut::max_index_i);
  }
  
  stdx::native_simd<T> val_A;
  stdx::native_simd<T> val_B;

  for (size_t i; i < stdx::native_simd<T>::size(); ++i) {
    val_A[i] = copal::lut::get<T>()[index_A[i]];
    val_B[i] = copal::lut::get<T>()[index_B[i]];
  }
  stdx::native_simd<T> lerpAmount = index_f - index_Af;
  return lerp(val_A, val_B, lerpAmount);
}

}
