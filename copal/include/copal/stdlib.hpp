#pragma once

#include <concepts>
#include <type_traits>
#include <utility>

#include "num.hpp"
#include "sin_lut.hpp"

namespace copal {

template<std::floating_point Type>
struct Stdlib {
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
T Stdlib<T>::sin_lookup(T xIn) {
  auto [x, sign] = angle_normalization_pi_over_2(xIn);
  T index_f  = x * (lut::max_index / copal::num::pi_over_2<T>);
  size_t index_A = static_cast<size_t>(index_f);

  if (index_A > copal::lut::max_index) {
    return copal::lut::get<T>()[copal::lut::max_index];
  }

  size_t index_B = index_A + 1;
  T lerpAmount = index_f - index_A;

  return sign * lerp(
    copal::lut::get<T>()[index_A],
    copal::lut::get<T>()[index_B],
    lerpAmount
  );
}

template<std::floating_point T>
T Stdlib<T>::sin_taylor(T xIn) {
  return Stdlib<T>::sin_taylor_terms<5>(xIn);
}

template<std::floating_point T>
template<size_t terms>
T Stdlib<T>::sin_taylor_terms(T xIn) {
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
T Stdlib<T>::sin_stdlib(T x) {
  if constexpr (std::is_same_v<T, float>) {
    return std::sinf(x);
  } else if constexpr (std::is_same_v<T, double>) {
    return std::sin(x);
  } else if constexpr (std::is_same_v<T, long double>) {
    return std::sinl(x);
  } else {
    static_assert(std::is_floating_point_v<T>, "std_sin requires a floating point type.");
  }
}

template<std::floating_point T>
T Stdlib<T>::fabs(T x) {
  if constexpr (std::is_same_v<T, float>) {
    return std::fabsf(x);
  } else if constexpr (std::is_same_v<T, double>) {
    return std::fabs(x);
  } else if constexpr (std::is_same_v<T, long double>) {
    return std::fabsl(x);
  } else {
    static_assert(std::is_floating_point_v<T>, "copal::stdlib::fabs requires a floating point type.");
  }
}

template<std::floating_point T>
T Stdlib<T>::fmod(T a, T b) {
  if constexpr (std::is_same_v<T, float>) {
    return std::fmodf(a, b);
  } else if constexpr (std::is_same_v<T, double>) {
    return std::fmod(a, b);
  } else if constexpr (std::is_same_v<T, long double>) {
    return std::fmodl(a, b);
  } else {
    static_assert(std::is_floating_point_v<T>, "copal::stdlib::fmod requires a floating point type.");
  }
}

template<std::floating_point T>
T Stdlib<T>::lerp(T a, T b, T lerpFactor) {
  // for uniform behaviour with infinity across copal::Stdlib, copal::Scalar, copal::VectorImpl, copal::VectorStdx
  if (std::isinf(a) || std::isinf(b) || std::isinf(lerpFactor))
    return a*b*lerpFactor;

  T result = std::lerp<T>(a, b, lerpFactor);
  return result;
}

template<std::floating_point T>
T Stdlib<T>::floor(T x) {
  if constexpr (std::is_same_v<T, float>) {
    return std::floorf(x);
  } else if constexpr (std::is_same_v<T, double>) {
    return std::floor(x);
  } else if constexpr (std::is_same_v<T, long double>) {
    return std::floorl(x);
  } else {
    static_assert(std::is_floating_point_v<T>, "copal::stdlib::floor requires a floating point type.");
  }
}

template<std::floating_point T>
std::pair<T, T> Stdlib<T>::angle_normalization_pi_over_2(T x) {
  constexpr T halfPi = copal::num::pi_over_2<T>;
  constexpr T pi     = copal::num::pi_x_1<T>;
  constexpr T twoPi  = copal::num::pi_x_2<T>;

  T sign = 1;
  
  if (x >= twoPi || x <= -twoPi)
    x = Stdlib<T>::fmod(x, twoPi);

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


/*


╰$ make -j
[ 16%] Building CXX object CMakeFiles/benchmark.dir/benchmark/sin.cpp.o
[ 33%] Building CXX object CMakeFiles/test.dir/test/math.test.cc.o
[ 50%] Building CXX object CMakeFiles/test.dir/test/sin.test.cc.o
^[rIn file included from /home/ra/code/cpp-deps/copal/benchmark/sin.cpp:15:
/home/ra/code/cpp-deps/copal/include/impl_scalar.hpp: In instantiation of ‘static T copal::Scalar<Type>::lerp(T, T, T) [with Type = float; T = float]’:
/home/ra/code/cpp-deps/copal/include/impl_scalar.hpp:45:32:   required from ‘static T copal::Scalar<Type>::sin_lookup(T) [with Type = float; T = float]’
   45 |   return sign * Scalar<T>::lerp(
      |                 ~~~~~~~~~~~~~~~^
   46 |     lut::get<T>()[index_A],
      |     ~~~~~~~~~~~~~~~~~~~~~~~     
   47 |     lut::get<T>()[index_B],
      |     ~~~~~~~~~~~~~~~~~~~~~~~     
   48 |     lerpAmount
      |     ~~~~~~~~~~                  
   49 |   );
      |   ~                             
/home/ra/code/cpp-deps/copal/benchmark/sin.cpp:67:1:   required from here
   67 | BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  1, 4, copal::Scalar<float>::sin_lookup>>);
      |                                                                                           ^~~~~~~~~~
/home/ra/code/cpp-deps/copal/include/impl_scalar.hpp:94:51: error: no matching function for call to ‘isinf(float (&)(copal::Scalar<float>::T, copal::Scalar<float>::T, copal::Scalar<float>::T))’
   94 |   if (std::isinf(a) || std::isinf(b) || std::isinf(lerp))
      |                                         ~~~~~~~~~~^~~~~~
In file included from /usr/include/c++/14.1.1/experimental/bits/simd.h:38,
                 from /usr/include/c++/14.1.1/experimental/simd:74,
                 from /home/ra/code/cpp-deps/copal/benchmark/sin.cpp:3:
/usr/include/c++/14.1.1/cmath:1169:5: note: candidate: ‘template<class _Tp> constexpr typename __gnu_cxx::__enable_if<std::__is_integer<_Tp>::__value, bool>::__type std::isinf(_Tp)’
 1169 |     isinf(_Tp)
      |     ^~~~~
/usr/include/c++/14.1.1/cmath:1169:5: note:   template argument deduction/substitution failed:
/usr/include/c++/14.1.1/cmath: In substitution of ‘template<class _Tp> constexpr typename __gnu_cxx::__enable_if<std::__is_integer<_Tp>::__value, bool>::__type std::isinf(_Tp) [with _Tp = float (*)(float, float, float)]’:
/home/ra/code/cpp-deps/copal/include/impl_scalar.hpp:94:51:   required from ‘static T copal::Scalar<Type>::lerp(T, T, T) [with Type = float; T = float]’
   94 |   if (std::isinf(a) || std::isinf(b) || std::isinf(lerp))


*/