#pragma once


#include <concepts>
#include "experimental/simd"
#include <experimental/bits/simd.h>
#include <experimental/bits/simd_math.h>
#include <type_traits>

#include "num.hpp"
#include "vector_stdx/math.hpp"
#include "stdx_definition.hpp"

namespace copal::vector_impl {
    

template<std::floating_point T>
stdx::native_simd<T> fabs(stdx::native_simd<T>& data) {
  stdx::native_simd_mask<T> lt_zero = data < T(0);
  return lt_zero ? -data : data;
}

template<std::floating_point T>
stdx::native_simd<T> floor(const stdx::native_simd<T>& x) {
  return vector_stdx::floor(x);
}

template<std::floating_point T>
stdx::native_simd<T> fmod(const stdx::native_simd<T>& a, const stdx::native_simd<T>& b) {
  stdx::native_simd_mask<T> a_lt_zero = a < T(0);
  stdx::native_simd<T> sign = T(1);
  if (stdx::any_of(a_lt_zero)) {
    stdx::where(a_lt_zero, sign) = T(-1);
  }

  auto a_abs = fabs(a);
  auto b_abs = fabs(b);

  stdx::native_simd<T> result;

  stdx::native_simd_mask<T> a_gt_b = a_abs > b_abs;
  if (stdx::any_of(a_gt_b)) {
    stdx::native_simd<T> fitTimes = a_abs / b_abs;
    stdx::native_simd<T> remainder = fitTimes - floor(fitTimes);
    stdx::where(a_gt_b, result) = remainder * b_abs * sign;
  }
  return result;
}

template<std::floating_point T>
stdx::native_simd<T> lerp(const stdx::native_simd<T>& a, const stdx::native_simd<T>& b, const stdx::native_simd<T>& lerp) {
  return (T(1) - lerp) * a + lerp * b;
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