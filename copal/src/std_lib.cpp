#include <concepts>
#include <type_traits>
#include <utility>

#include "stdlib.hpp"
#include "num.hpp"
#include "sin_lut.hpp"


namespace copal {

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

template class Stdlib<float>;
template class Stdlib<double>;

}
