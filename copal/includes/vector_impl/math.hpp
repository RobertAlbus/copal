#pragma once


#include <concepts>
#include "experimental/simd"
#include <experimental/bits/simd.h>
#include <experimental/bits/simd_math.h>
#include <type_traits>

namespace copal::vector_impl {
    
namespace stdx = std::experimental;

template<std::floating_point T>
stdx::native_simd<T> fabs(stdx::native_simd<T>& data) {
  stdx::native_simd_mask<T> lt_zero = data < T(0);
  return lt_zero ? -data : data;
}

template<std::floating_point T>
stdx::native_simd<T> floor(const stdx::native_simd<T>& x) {
  return stdx::static_simd_cast<stdx::native_simd<T>>(
    stdx::static_simd_cast<stdx::native_simd<int>>(x)
  );
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
    stdx::native_simd<T> remainder = fitTimes - copal::vector_impl::floor(fitTimes);
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
normalize_angle_pi_over_2(const stdx::native_simd<T>& x) {
  const stdx::native_simd<T> halfPi = std::numbers::pi_v<T> * T(0.5);
  const stdx::native_simd<T> pi     = std::numbers::pi_v<T>;
  const stdx::native_simd<T> twoPi  = std::numbers::pi_v<T> * T(2);
  
  stdx::native_simd<T> sign = T(1);
  stdx::native_simd<T> xOut = copal::vector_impl::fmod<T>(x, twoPi);

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