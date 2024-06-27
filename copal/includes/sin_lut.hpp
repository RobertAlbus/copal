#pragma once

#include <array>
#include <cmath>
#include <concepts>
#include <experimental/bits/simd.h>
#include <numbers>
#include <type_traits>

#include "stdlib/sin_stdlib.hpp"

namespace copal::lut {

template<std::floating_point T, std::size_t size>
std::array<T, size> create_sin_lut() {
  std::array<T, size> lut;
  constexpr T rangeMin = 0;
  constexpr T rangeMax = std::numbers::pi_v<T> * T(0.5);
  constexpr T segments = static_cast<T>(size - 1); // segments = indices-1
  constexpr T segmentsReciprocal = T(1) / segments;
  
  for (size_t i = 0; i < size; ++i) {
    T position = static_cast<T>(i) * segmentsReciprocal;
    lut[i] = copal::stdlib::sin_stdlib<T>(position);
  }

  return lut;
}

constexpr int    size_i              = 2048;
constexpr int    max_index_i         = size_i - 1;
constexpr int    max_index_minus_1_i = size_i - 2;
constexpr size_t size                = size_i;
constexpr size_t max_index           = max_index_i;
constexpr size_t max_index_minus_1   = max_index_minus_1_i;

const std::array<float,       size> sinLUT2048_f  = create_sin_lut<float,       size>();
const std::array<double,      size> sinLUT2048_d  = create_sin_lut<double,      size>();
const std::array<long double, size> sinLUT2048_ld = create_sin_lut<long double, size>();

template<std::floating_point T>
const std::array<T, size>& get() {
    if constexpr (std::is_same_v<T, float>) {
      return sinLUT2048_f;
    } else if constexpr (std::is_same_v<T, double>) {
      return sinLUT2048_d;
    } else if constexpr (std::is_same_v<T, long double>) {
      return sinLUT2048_ld;
    } else {
      static_assert(std::is_floating_point_v<T>, "copal::lut::get<T> requires a floating point type.");
    }
}

}
