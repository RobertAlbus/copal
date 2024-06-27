#pragma once

#include "benchmark/benchmark.h"
#include "math.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <experimental/simd>
#include <experimental/bits/simd.h>
#include <experimental/bits/simd_math.h>
#include <type_traits>

#include "math.hpp"
#include "sin_lut.hpp"
#include "stdx_definition.hpp"

namespace copal::vector_impl {

template<std::floating_point T>
stdx::native_simd<T> sin_taylor(const stdx::native_simd<T>& xInput) {
    auto [x, sign] = normalize_angle_pi_over_2(xInput);
    stdx::native_simd<T> term = x;
    stdx::native_simd<T> sum  = term;
    
    // 5 terms for accuracy within 10^-7 for {0 < x < 0.25 pi} 
    for (uint m = 2; m <= 10; m += 2) {
      term *= -x * x / T(m * (m + 1));
      sum += term;
    }
    return sum * sign;
}

template<std::floating_point T>
void sin_taylor(T* x_input, T* x_output, size_t n) {
  stdx::native_simd<T>* simd_x     = reinterpret_cast<stdx::native_simd<T>*>(x_input);
  stdx::native_simd<T>* simd_x_out = reinterpret_cast<stdx::native_simd<T>*>(x_output);
  
  const std::size_t size = stdx::native_simd<T>::size();
  for(std::size_t i = 0; i + size <= n; i += size) {
    size_t idx = i / size;
    stdx::native_simd<T> chunk_x = simd_x[idx];
    benchmark::DoNotOptimize(simd_x_out[idx] = sin_taylor(chunk_x));
  }
}


template<std::floating_point T>
stdx::native_simd<T> sin_lookup(const stdx::native_simd<T>& xInput) {
  auto [x, sign] = normalize_angle_pi_over_2(xInput);

  stdx::native_simd<T> term = x;
  stdx::native_simd<T> sum  = term;

  // if constexpr (std::is_same_v<T, float>) {
  //     // do not use lerp for single precision float.
  //     // precision in table size 2048 for [0 .. 0.25 pi] 
  //     // is same as rounding error
  //     return sign * copal::lut::get<T>()[index_A];
  // }

  stdx::native_simd<T>   index_f  = x * T(lut::max_index);
  stdx::native_simd<int> index_A  = stdx::static_simd_cast<stdx::native_simd<int>>(index_f);
  stdx::native_simd<T>   index_Af = stdx::static_simd_cast<stdx::native_simd<T>>(index_A);
  stdx::native_simd<int> index_B  = ++index_A;

  stdx::native_simd<int> maxIndex_i = copal::lut::max_index_i;
  stdx::native_simd<T>   maxIndex   = T(copal::lut::max_index_i);
  stdx::native_simd_mask<int> invalidIndex_i = index_A  >= maxIndex_i;
  stdx::native_simd_mask<T>   invalidIndex   = index_Af >= maxIndex;
  if (stdx::any_of(invalidIndex_i)) {
    stdx::where(invalidIndex,   index_f) = maxIndex;
    stdx::where(invalidIndex_i, index_A) = maxIndex_i;
    stdx::where(invalidIndex_i, index_B) = maxIndex_i;
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

template<std::floating_point T>
void sin_lookup(T* x_input, T* x_output, size_t n) {
  stdx::native_simd<T>* simd_x     = reinterpret_cast<stdx::native_simd<T>*>(x_input);
  stdx::native_simd<T>* simd_x_out = reinterpret_cast<stdx::native_simd<T>*>(x_output);
  
  const std::size_t size = stdx::native_simd<T>::size();
  for(std::size_t i = 0; i + size <= n; i += size) {
    size_t idx = i / size;
    stdx::native_simd<T> chunk_x = simd_x[idx];
    benchmark::DoNotOptimize(simd_x_out[idx] = sin_lookup(chunk_x));
  }
}

}
