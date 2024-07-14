#pragma once

#include <array>
#include <cmath>
#include <concepts>
#include <experimental/simd>
#include <experimental/bits/simd.h>
#include <type_traits>

#include "num.hpp"
#include "stdx_definition.hpp"

namespace copal::lut {

// // original size 2^11 | 2048   * [4,8] bytes is [8.2, 16.4] kilobytes
// constexpr int    size_i              = 2048;
// // desired size  2^18 | 262144 * [4,8] bytes is [1.1, 2.1] megabytes
// constexpr int    size_i              = 2 << 18;

// required size 2^22 | 4194304 * [4,8] bytes is [16.8, 33.55] megabytes
constexpr int    size_i              = 2 << 22;
constexpr int    max_index_i         = size_i - 1;
constexpr int    max_index_minus_1_i = size_i - 2;
constexpr size_t size                = size_i;
constexpr size_t max_index           = max_index_i;
constexpr size_t max_index_minus_1   = max_index_minus_1_i;

template<std::floating_point T, std::size_t size>
std::array<T, size> create_sin_lut() {
  std::array<T, size> lut;
  constexpr T rangeMin = 0;
  constexpr T rangeMax = num::pi_over_2<T>;
  constexpr T segments = static_cast<T>(size); // segments = indices-1
  constexpr T segmentsReciprocal = T(1) / segments;
  
  for (size_t i = 0; i < size; ++i) {
    T position = static_cast<T>(i) * segmentsReciprocal * rangeMax;

    if constexpr(std::is_same_v<T, float>) {
      lut[i] = std::sinf(position);    
    } else if constexpr(std::is_same_v<T, double>) {
      lut[i] = std::sin(position);    
    } else if constexpr(std::is_same_v<T, float>) {
      lut[i] = std::sinl(position);    
    }
  }

  return lut;
}

alignas(stdx::memory_alignment_v<stdx::native_simd<float>>)
const std::array<float,       size> sinLUT_f  = create_sin_lut<float,       size>();

alignas(stdx::memory_alignment_v<stdx::native_simd<double>>)
const std::array<double,      size> sinLUT_d  = create_sin_lut<double,      size>();

alignas(stdx::memory_alignment_v<stdx::native_simd<long double>>)
const std::array<long double, size> sinLUT_ld = create_sin_lut<long double, size>();

template<std::floating_point T>
const std::array<T, size>& get() {
  if constexpr (std::is_same_v<T, float>) {
    return sinLUT_f;
  } else if constexpr (std::is_same_v<T, double>) {
    return sinLUT_d;
  } else if constexpr (std::is_same_v<T, long double>) {
    return sinLUT_ld;
  } else {
    static_assert(std::is_floating_point_v<T>, "copal::lut::get<T> requires a floating point type.");
  }
}

}
