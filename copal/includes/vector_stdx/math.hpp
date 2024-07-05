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
  stdx::native_simd<T> result = stdx::fmod(a, b);

  // stdx::native_simd_mask<T> lt_zero = result < 0;
  // if(stdx::any_of(lt_zero)) {
  //   where(lt_zero, result) = result += b;
  // }

  return result;
}

template<std::floating_point T>
stdx::native_simd<T> lerp(const stdx::native_simd<T>& a, const stdx::native_simd<T>& b, const stdx::native_simd<T>& lerp) {
  stdx::native_simd<T> result;
  stdx::native_simd_mask<T> infinite = stdx::isinf<>(a) || stdx::isinf<>(b) || stdx::isinf<>(lerp);

  if (stdx::any_of(infinite))
    stdx::where(infinite, result) = a*b*lerp;

  stdx::where(!infinite, result) = (T(1) - lerp) * a + lerp * b;
  return result;
}

template<std::floating_point T>
stdx::native_simd<T> floor(const stdx::native_simd<T>& a) {
  return stdx::floor<T>(a);
}

template<std::floating_point T>
std::pair<stdx::native_simd<T>, stdx::native_simd<T>>
angle_normalization_pi_over_2(const stdx::native_simd<T>& xIn) {
  constexpr T halfPi = num::pi_over_2<T>;
  constexpr T pi     = num::pi_x_1<T>;
  constexpr T twoPi  = num::pi_x_2<T>;

  stdx::native_simd<T> x(xIn);

  stdx::native_simd_mask<T> gt_2pi = x >= twoPi || x <= -twoPi;
  if(stdx::any_of(gt_2pi)) {
    where(gt_2pi, x) = copal::vector_stdx::fmod<T>(x, twoPi);
  }

  stdx::native_simd_mask<T> lt_zero = x < 0;
  if(stdx::any_of(lt_zero)) {
    where(lt_zero, x) = x + twoPi;
  }

  stdx::native_simd<T> sign = 1;
  stdx::native_simd_mask<T> gt_pi = x >= pi;
  if(stdx::any_of(gt_pi)) {
    where(gt_pi, x) = x - pi;
    where(gt_pi, sign) = -1;
  }

  stdx::native_simd_mask<T> gt_halfPi = x >= halfPi;
  if(stdx::any_of(gt_halfPi)) {
    where(gt_halfPi, x) = pi - x;
  }

  return { x, sign };
}

}