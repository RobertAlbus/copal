#pragma once

#include <concepts>
#include <type_traits>

#include "stdx_definition.hpp"

namespace copal {
template<std::floating_point Type>
struct VectorImpl {
  using T = typename stdx::native_simd<Type>;
  using Float = Type;

  static T sin_lookup(T x);
  static T sin_taylor(T x);
  template<size_t terms = 5>
  static T sin_taylor_terms(T x);
  static T sin_stdlib(T x);

  static T fabs(T x);
  static T fmod(T a, T b);
  static T lerp(T a, T b, T lerpFactor);
  static T floor(T x);
  static std::pair<T, T> angle_normalization_pi_over_2(T x);
};

}
