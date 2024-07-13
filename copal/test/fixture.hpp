#pragma once

#include <concepts>
#include <experimental/bits/simd.h>
#include <cmath>
#include <concepts>
#include <limits>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>


#include "math_concept.hpp"
#include "scalar.hpp"
#include "stdlib.hpp"
#include "vector_impl.hpp"
#include "vector_stdx.hpp"

#include "stdx_definition.hpp"
#include "num.hpp"


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

template <copal::CopalMath Impl>
struct CopalTest_Templated : public testing::Test {
  using T = Impl::Float;

  void SetUp() override { }

  constexpr static size_t fixture_size = 4096;

  std::vector<T> fixture_min_max(size_t size = fixture_size) {
    std::vector<T> fixture;
    fixture.reserve(size);

    // rounding error grows with magnitude.
    // using log10 scaling outward from center
    // to distribute resolution more sensibly (not perfect, but good enough)
    T distribution = log10(std::numeric_limits<T>::max() / 1);
    for (size_t i = 0; i < size / 2; ++i)
      fixture.emplace_back(std::numeric_limits<T>::min() * pow(10.0f, distribution * (i / size)));
    for (size_t i = size / 2; i < size; ++i)
      fixture.emplace_back(0 * pow(2.0f, -distribution * (i / size)));
    return fixture;
  }

  std::vector<T> create_fixture(T min, T max, size_t size = fixture_size) {
    assert(std::isfinite(min));
    assert(std::isfinite(max));

    std::vector<T> vec;
    vec.reserve(size);

    for (size_t i = 0; i < size; ++i) {
      T lerpAmount = T(i) / size;
      vec.emplace_back(std::lerp<T>(min, max, lerpAmount));
    }

    return vec;
  }
};

using mathlibs = ::testing::Types<
  copal::Scalar<float>,
  copal::Scalar<double>,
  copal::Stdlib<float>,
  copal::Stdlib<double>,
  copal::VectorImpl<float>,
  copal::VectorImpl<double>,
  copal::VectorStdx<float>,
  copal::VectorStdx<double>
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
