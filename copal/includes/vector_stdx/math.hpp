#pragma once

#include <cmath>
#include <concepts>
#include <experimental/simd>
#include <experimental/bits/simd.h>
#include <experimental/bits/simd_math.h>
#include <experimental/bits/simd_scalar.h>
#include <experimental/bits/simd_builtin.h>
#include <type_traits>

#include "num.hpp"
#include "stdx_definition.hpp"

namespace copal::vector_impl {
// forward declare due to cyclic dependency between files.
// copal::vector_impl::floor => copal::vector_stdx::floor
// copal::vector_stdx::lerp => copal::vector_impl::lerp
  template<std::floating_point T>
  stdx::native_simd<T> lerp(const stdx::native_simd<T>& a, const stdx::native_simd<T>& b, const stdx::native_simd<T>& lerpAmount);
}

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
angle_normalization_pi_over_2(const stdx::native_simd<T>& x) {
  const stdx::native_simd<T> halfPi = num::pi_over_2<T>;
  const stdx::native_simd<T> pi     = num::pi_x_1<T>;
  const stdx::native_simd<T> twoPi  = num::pi_x_2<T>;
  
  stdx::native_simd<T> xOut = x;

  stdx::native_simd_mask<T> gt_2pi = xOut > twoPi || xOut < -twoPi;
  if(stdx::any_of(gt_2pi)) {
    where(gt_2pi, xOut) = fmod<T>(x, twoPi);
  }

  stdx::native_simd_mask<T> lt_zero = xOut < 0;
  if(stdx::any_of(lt_zero)) {
    where(lt_zero, xOut) = xOut += twoPi;
  }

  stdx::native_simd<T> sign = T(1);
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