#pragma once

#include <cmath>
#include <concepts>
#include <experimental/simd>
#include <experimental/bits/simd.h>
#include <experimental/bits/simd_math.h>
#include <experimental/bits/simd_scalar.h>
#include <experimental/bits/simd_builtin.h>
#include <type_traits>

#include "stdx_definition.hpp"
#include "vector_impl/math.hpp"

namespace copal::vector_stdx {
    
namespace stdx = std::experimental;

template<std::floating_point T>
stdx::native_simd<T> fabs(const stdx::native_simd<T>& x) {
  return stdx::fabs(x);
}

template<std::floating_point T>
stdx::native_simd<T> fmod(const stdx::native_simd<T>& a, const stdx::native_simd<T>& b) {
  return stdx::fmod(a, b);
}

template<std::floating_point T>
stdx::native_simd<T> lerp(const stdx::native_simd<T>& a, const stdx::native_simd<T>& b, const stdx::native_simd<T>& lerpAmount) {
  // std::lerp overload missing
  return copal::vector_impl::lerp(a, b, lerpAmount);
}

template<std::floating_point T>
stdx::native_simd<T> floor(const stdx::native_simd<T>& a) {
  return stdx::floor<T>(a);
}

template<std::floating_point T>
std::pair<stdx::native_simd<T>, stdx::native_simd<T>>
normalize_angle_pi_over_2(const stdx::native_simd<T>& x) {
  const stdx::native_simd<T> halfPi = std::numbers::pi_v<T> * T(0.5);
  const stdx::native_simd<T> pi     = std::numbers::pi_v<T>;
  const stdx::native_simd<T> twoPi  = std::numbers::pi_v<T> * T(2);
  
  stdx::native_simd<T> sign = T(1);
  stdx::native_simd<T> xOut = copal::vector_stdx::fmod<T>(x, twoPi);

  stdx::native_simd_mask<T> gt_pi = xOut > pi;
  if(stdx::any_of(gt_pi)) {
    where(gt_pi, xOut) = xOut - pi;
    where(gt_pi, sign) = T(-1);
  }

  stdx::native_simd_mask<T> gt_halfPi = xOut > halfPi;
  if(stdx::any_of(gt_halfPi)) {
    where(gt_pi, xOut) = pi - xOut;
  }
  return { xOut, sign };
}

}