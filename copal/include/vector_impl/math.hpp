#pragma once


#include <concepts>
#include "experimental/simd"
#include <experimental/bits/simd.h>
#include <experimental/bits/simd_math.h>
#include <iostream>
#include <type_traits>

#include "num.hpp"
#include "vector_stdx/math.hpp"
#include "stdx_definition.hpp"

namespace copal::vector_impl {

template<std::floating_point T>
stdx::native_simd<T> fabs(const stdx::native_simd<T>& data) {
  stdx::native_simd<T> result = data;
  stdx::native_simd_mask<T> lt_zero = data < 0;
  if (stdx::any_of(lt_zero)) {
    stdx::where(lt_zero, result) = -data;
  }
  return result;
}

template<std::floating_point T>
stdx::native_simd<T> floor(const stdx::native_simd<T>& x) {
  return x - fmod(x, {1});
}

template<std::floating_point T>
stdx::native_simd<T> fmod(const stdx::native_simd<T>& a, const stdx::native_simd<T>& b) {
  stdx::native_simd<T> sign = 1;
  stdx::native_simd_mask<T> a_lt_zero = a < 0;
  if (stdx::any_of(a_lt_zero)) {
    stdx::where(a_lt_zero, sign) = -1;
  }

  auto a_abs = copal::vector_impl::fabs(a);
  auto b_abs = copal::vector_impl::fabs(b);

  stdx::native_simd<T> result {a};

  stdx::native_simd_mask<T> a_eq_b = a_abs == b_abs;
  if (stdx::any_of(a_eq_b)) {
    stdx::where(a_eq_b, result) = 0;
  }

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
  stdx::native_simd<T> result;
  stdx::native_simd_mask<T> infinite = stdx::isinf<>(a) || stdx::isinf<>(b) || stdx::isinf<>(lerp);

  if (stdx::any_of(infinite))
    stdx::where(infinite, result) = a*b*lerp;

  stdx::where(!infinite, result) = (T(1) - lerp) * a + lerp * b;
  return result;
}

template<std::floating_point T>
std::pair<stdx::native_simd<T>, stdx::native_simd<T>>
angle_normalization_pi_over_2(const stdx::native_simd<T>& xInput) {
  constexpr T halfPi = num::pi_over_2<T>;
  constexpr T pi     = num::pi_x_1<T>;
  constexpr T twoPi  = num::pi_x_2<T>;

  stdx::native_simd<T> x(xInput);

  stdx::native_simd_mask<T> gt_2pi = x >= twoPi || x <= -twoPi;
  if(stdx::any_of(gt_2pi)) {
    where(gt_2pi, x) = copal::vector_impl::fmod<T>(x, twoPi);
  }

  stdx::native_simd_mask<T> lt_zero = x < 0;
  if(stdx::any_of(lt_zero)) {
    where(lt_zero, x) = x + twoPi;
  }

  stdx::native_simd<T> sign = 1;
  stdx::native_simd_mask<T> gte_pi = x >= pi;
  if(stdx::any_of(gte_pi)) {
    where(gte_pi, x) = x - pi;
    where(gte_pi, sign) = -1;
  }

  stdx::native_simd_mask<T> gt_halfPi = x >= halfPi;
  if(stdx::any_of(gt_halfPi)) {
    where(gt_halfPi, x) = pi - x;
  }

  return { x, sign };
}

}