#pragma once

#include <concepts>
#include <type_traits>

#include "stdlib.hpp"
#include "num.hpp"
#include "sin_lut.hpp"

namespace copal {

template<std::floating_point Type>
struct Scalar {
  using T     = Type;
  using Float = Type;

  static T sin_lookup(T x);

  static T sin_taylor(T x);
  template<size_t terms = 5>
  static T sin_taylor_terms(T x);

  static T sin_stdlib(T x);
  static T fabs(T xIn);
  static T fmod(T a, T b);
  static T lerp(T a, T b, T lerpFactor);
  static T floor(T x);
  static std::pair<T, T> angle_normalization_pi_over_2(T x);
};

template<std::floating_point T>
T Scalar<T>::sin_lookup(T xIn) {
  auto [x, sign] = angle_normalization_pi_over_2(xIn);
  T index_f  = x * (lut::max_index / num::pi_over_2<T>);
  size_t index_A = static_cast<size_t>(index_f);

  if (index_A > lut::max_index) {
    return lut::get<T>()[lut::max_index];
  }

  size_t index_B = index_A + 1;
  T lerpAmount = index_f - index_A;

  return sign * Scalar<T>::lerp(
    lut::get<T>()[index_A],
    lut::get<T>()[index_B],
    lerpAmount
  );
}

template<std::floating_point T>
T Scalar<T>::sin_taylor(T xIn) {
  return Scalar<T>::sin_taylor_terms<5>(xIn);
}

template<std::floating_point T>
template<size_t terms>
T Scalar<T>::sin_taylor_terms(T xIn) {
  auto [x, sign] = angle_normalization_pi_over_2(xIn);
  T term = x;
  T sum = term;

  constexpr size_t termsEnd = terms * 2;
  for (size_t m = 2; m <= termsEnd; m += 2) {
    term *= -x * x / (m * (m + 1));
    sum += term;
  }

  return sign * sum;
}

template<std::floating_point T>
T Scalar<T>::sin_stdlib(T x) {
  return Stdlib<T>::sin_stdlib(x);
}

template<std::floating_point T>
T Scalar<T>::fabs(T x) {
  return x < 0 ? -x : x;
}

template<std::floating_point T>
T Scalar<T>::fmod(T a, T b) {
  // falls back to copal::stdlib::fmod
  // see notes: Imprecision - FMOD

  // TODO: replace with actual impl like in vectorized version?
  return Stdlib<T>::fmod(a, b);
}

template<std::floating_point T>
T Scalar<T>::lerp(T a, T b, T lerpFactor) {
  // for uniform behaviour with infinity across copal::Stdlib, copal::Scalar, copal::VectorImpl, copal::VectorStdx
  if (std::isinf(a) || std::isinf(b) || std::isinf(lerpFactor))
    return a*b*lerpFactor;

  return (1 - lerpFactor) * a + lerpFactor * b;
}

template<std::floating_point T>
T Scalar<T>::floor(T x) {
  T fractionalPart = fmod(x, {1});
  T integerPart    = x - fractionalPart;
  if (x < 0 && x != integerPart) {
      return integerPart - 1;
  }
  return integerPart;
}

template<std::floating_point T>
std::pair<T, T> Scalar<T>::angle_normalization_pi_over_2(T x) {
  constexpr T halfPi = num::pi_over_2<T>;
  constexpr T pi     = num::pi_x_1<T>;
  constexpr T twoPi  = num::pi_x_2<T>;

  T sign = 1;
  
  if (x >= twoPi || x <= -twoPi)
    x = Scalar<T>::fmod(x, twoPi);

  if (x < 0)
    x += twoPi;

  if (x >= pi) {
    x -= pi;
    sign = -1;
  }

  if (x >= halfPi)
    x = pi - x;

  return { x, sign };
}

}