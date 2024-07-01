#pragma once

#include "num.hpp"
#include <cmath>
#include <concepts>
#include <limits>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>


template <std::floating_point T>
struct tolerance {
  static constexpr T fmod = 10e5 * std::numeric_limits<T>::epsilon();
  static constexpr T angle_normalization_is_symmetrical_x_2 = 1.2e-7;
  static constexpr T angle_normalization_is_symmetrical_x_3 = 2.4e-7;
  static constexpr T angle_normalization_is_symmetrical_x_4 = 2.4e-7;
  static constexpr T angle_normalization_is_periodic        = 7.0e-6;
  static constexpr T sin_lookup_float                       = 2.4e-7;
  static constexpr T sin_lookup_double                      = 1.4e-7;
  static constexpr T sin_taylor_float                       = 1.8e-7;
  static constexpr T sin_taylor_double                      = 1.0e-7;
};

template <std::floating_point T>
struct CopalTest : public testing::Test {
  void SetUp() override { }

  constexpr static size_t fixture_size = 4096;

  std::vector<T> fixture_min_max(size_t size = fixture_size) {
    return create_fixture(
      std::numeric_limits<T>::min(),
      std::numeric_limits<T>::max(),
      size
    );
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

};

using fpm = ::testing::Types<float, double>;
TYPED_TEST_SUITE(CopalTest, fpm);

TYPED_TEST(CopalTest, fixture) {
  using T = TypeParam;

  size_t numItems = 21;
  size_t end   = numItems / 2;
  size_t start = -end;
  std::vector<T> fixture = this->create_fixture(start, end, numItems);

  EXPECT_EQ(fixture.size(), numItems);
  for (size_t i = start; i < end; ++i) {
    EXPECT_EQ(i, fixture[i]);
  }
}
