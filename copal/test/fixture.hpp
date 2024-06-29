#pragma once

#include "num.hpp"
#include <cmath>
#include <concepts>
#include <limits>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>


template <std::floating_point T>
struct CopalTest : public testing::Test {
  void SetUp() override { }

  constexpr static size_t fixture_size = 4096;

  std::vector<T> fixture_min_max(size_t size = fixture_size) {
    return createfixture(
      std::numeric_limits<T>::min(),
      std::numeric_limits<T>::max(),
      size
    );
  }
  std::vector<T> fixture_quarter_cycle(size_t size = fixture_size + 1 /* up to but not including*/) {
    return createfixture(0, copal::num::pi_over_2<T>, size);
  }
  std::vector<T> fixture_half_cycle(size_t size = fixture_size + 1 /* up to but not including*/) {
    return createfixture(0, copal::num::pi_x_1<T>, size);
  }
  std::vector<T> fixture_full_cycle(size_t size = fixture_size + 1 /* up to but not including*/) {
    return createfixture(0, copal::num::pi_x_2<T>, size);
  }

  std::vector<T> createfixture(T min, T max, size_t size = fixture_size) {
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
  std::vector<T> fixture = this->createfixture(start, end, numItems);

  EXPECT_EQ(fixture.size(), numItems);
  for (size_t i = start; i < end; ++i) {
    EXPECT_EQ(i, fixture[i]);
  }
}
