#pragma once

#include <concepts>
#include <experimental/bits/simd.h>
#include <cmath>
#include <concepts>
#include <limits>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>

#include "num.hpp"
#include "scalar/math.hpp"
#include "stdlib/math.hpp"
#include "vector_impl/math.hpp"
#include "vector_stdx/math.hpp"
#include "stdx_definition.hpp"


template <std::floating_point T>
struct tolerance {
  static constexpr T fmod = 10e5 * std::numeric_limits<T>::epsilon();
  static constexpr T angle_normalization_is_symmetrical_x_2 = 1.2e-7;
  static constexpr T angle_normalization_is_symmetrical_x_3 = 2.4e-7;
  static constexpr T angle_normalization_is_symmetrical_x_4 = 2.4e-7;
  static constexpr T simd_angle_normalization_is_symmetrical_x_2 = 1.2e-7;
  static constexpr T simd_angle_normalization_is_symmetrical_x_3 = std::is_same_v<T, float> ? 3.9e-4 : 2.4e-7; // boo :(
  static constexpr T simd_angle_normalization_is_symmetrical_x_4 = std::is_same_v<T, float> ? 3.9e-4 : 2.4e-7; // boo :(
  static constexpr T angle_normalization_is_periodic        = 7.0e-6;
  static constexpr T angle_norm_periodic_when_close = std::is_same_v<T, float> ? 1.5e-06 : 1.5e-16;
  static constexpr T sin_lookup_float                       = 2.4e-7;
  static constexpr T sin_lookup_double                      = 1.4e-7;
  static constexpr T sin_taylor_float                       = 1.8e-7;
  static constexpr T sin_taylor_double                      = 1.0e-7;
};

template<typename Impl>
concept CopalMath = requires {
    typename Impl::T;
    requires std::is_floating_point_v<typename Impl::T> || stdx::is_simd_v<typename Impl::T>;
    typename Impl::Float;
    requires std::is_floating_point_v<typename Impl::Float>;
    requires requires(Impl::T a, Impl::T b, Impl::T lerpFactor) {
        { Impl::fabs(a)                } -> std::same_as<typename Impl::T>;
        { Impl::fmod(a, b)             } -> std::same_as<typename Impl::T>;
        { Impl::lerp(a, b, lerpFactor) } -> std::same_as<typename Impl::T>;
        { Impl::floor(a)               } -> std::same_as<typename Impl::T>;
        { Impl::angle_normalization_pi_over_2(a) } -> std::same_as<std::pair<typename Impl::T, typename Impl::T>>;
    };

};

template<std::floating_point Type>
struct Scalar {
    using T     = Type;
    using Float = Type;

    static T fabs(T x)                    { return copal::scalar::fabs(x);                }
    static T fmod(T a, T b)               { return copal::scalar::fmod(a, b);             }
    static T lerp(T a, T b, T lerpFactor) { return copal::scalar::lerp(a, b, lerpFactor); }
    static T floor(T x)                   { return copal::scalar::floor(x);               }
    static std::pair<T, T> angle_normalization_pi_over_2(T x) { return copal::scalar::angle_normalization_pi_over_2(x); }
};

template<std::floating_point Type>
struct Stdlib {
    using T     = Type;
    using Float = Type;

    static T fabs(T x)                    { return copal::stdlib::fabs(x);                }
    static T fmod(T a, T b)               { return copal::stdlib::fmod(a, b);             }
    static T lerp(T a, T b, T lerpFactor) { return copal::stdlib::lerp(a, b, lerpFactor); }
    static T floor(T x)                   { return copal::stdlib::floor(x);               }
    static std::pair<T, T> angle_normalization_pi_over_2(T x) { return copal::scalar::angle_normalization_pi_over_2(x); }
};

template<std::floating_point Type>
struct VectorImpl {
    using T = typename stdx::native_simd<Type>;
    using Float = Type;

    static T fabs(T x)                    { return copal::vector_impl::fabs(x);                }
    static T fmod(T a, T b)               { return copal::vector_impl::fmod(a, b);             }
    static T lerp(T a, T b, T lerpFactor) { return copal::vector_impl::lerp(a, b, lerpFactor); }
    static T floor(T x)                   { return copal::vector_impl::floor(x);               }
    static std::pair<T, T> angle_normalization_pi_over_2(T x) { return copal::vector_impl::angle_normalization_pi_over_2(x); }
};

template<std::floating_point Type>
struct VectorStdx {
    using T = stdx::native_simd<Type>;
    using Float = Type;

    static T fabs(T x)                    { return copal::vector_stdx::fabs(x);                }
    static T fmod(T a, T b)               { return copal::vector_stdx::fmod(a, b);             }
    static T lerp(T a, T b, T lerpFactor) { return copal::vector_stdx::lerp(a, b, lerpFactor); }
    static T floor(T x)                   { return copal::vector_stdx::floor(x);               }
    static std::pair<T, T> angle_normalization_pi_over_2(T x)  { return copal::vector_stdx::angle_normalization_pi_over_2(x); }
};

template <CopalMath Impl>
struct CopalTest_Templated : public testing::Test {
  using T = Impl::Float;

  void SetUp() override { }

  constexpr static size_t fixture_size = 4096;

  std::vector<T> fixture_min_max(size_t size = fixture_size) {
    std::vector<T> fixture;
    fixture.reserve(size);

    // rounding error grows with magnitude.
    // using log2 scaling outward from center
    // to distribute resolution more sensibly 
    T linearized = log2(std::numeric_limits<T>::max() / 1);
    for (size_t i = 0; i < size / 2; ++i)
      fixture.emplace_back(std::numeric_limits<T>::min() * pow(2.0f, linearized * (i / size)));
    for (size_t i = size / 2; i < size; ++i)
      fixture.emplace_back(0 * pow(2.0f, -linearized * (i / size)));
    return fixture;
  }

  std::vector<T> create_fixture(T min, T max, size_t size = fixture_size) {
    assert(std::isfinite(min) && !std::isnan(min));
    assert(std::isfinite(max) && !std::isnan(max));

    std::vector<T> vec;
    vec.reserve(size);

    for (size_t i = 0; i < size; ++i) {
      T lerpAmount = T(i) / size;
      vec.emplace_back(std::lerp<T>(min, max, lerpAmount));
    }

    return vec;
  }

  std::array<T, 4096> create_fixture_array(T min, T max) {
    assert(std::isfinite(min) && !std::isnan(min));
    assert(std::isfinite(max) && !std::isnan(max));

    std::array<T, 4096> array;

    for (size_t i = 0; i < 4096; ++i) {
      T lerpAmount = T(i) / 4096;
      array[i] = std::lerp<T>(min, max, lerpAmount);
    }

    return array;
  }
};

using mathlibs = ::testing::Types<
  Scalar<float>,
  Scalar<double>,
  Stdlib<float>,
  Stdlib<double>,
  VectorImpl<float>,
  VectorImpl<double>,
  VectorStdx<float>,
  VectorStdx<double>
>;

TYPED_TEST_SUITE(CopalTest_Templated, mathlibs);

TYPED_TEST(CopalTest_Templated, fixture) {
  using mathlib = TypeParam;
  using T     = mathlib::T;
  using Float = mathlib::Float;

  size_t numItems = 21;
  size_t end   = numItems / 2;
  size_t start = -end;
  std::vector<Float> fixture = this->create_fixture(start, end, numItems);

  EXPECT_EQ(fixture.size(), numItems);
  for (size_t i = start; i < end; ++i) {
    EXPECT_EQ(i, fixture[i]);
  }
}
