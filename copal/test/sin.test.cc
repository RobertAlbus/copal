#include <array>
#include <cmath>
#include <experimental/simd>
#include <experimental/bits/simd.h>
#include <format>
#include <limits>
#include <numbers>
#include <ranges>
#include <type_traits>
#include <tuple>
#include <vector>

#include "gtest/gtest.h"

#include "fixture.hpp"


TYPED_TEST(CopalTest_Templated, sin_lookup) {
  using mathlib = TypeParam;
  using T       = mathlib::T;
  using Float   = mathlib::Float;

  std::vector<Float> inputs = this->fixture_min_max();

  std::function<Float(Float)> stdSin;

  if constexpr (std::is_same_v<T, float>) {
    for (size_t i; i < inputs.size(); ++i) {
      EXPECT_FLOAT_EQ(mathlib::sin_lookup(inputs[i]), std::sinf(inputs[i]));
    }
  } else if constexpr (std::is_same_v<T, double>) {
    for (size_t i; i < inputs.size(); ++i) {
      EXPECT_NEAR(mathlib::sin_lookup(inputs[i]), std::sin(inputs[i]), 2.4e-6);
    }
  } else if constexpr (stdx::is_simd_v<T>) {
    for (size_t i; i < inputs.size(); i += T::size()) {
      T input_chunk(&inputs[i], stdx::element_aligned);
      T result = mathlib::sin_lookup(input_chunk);
      T stdResult = stdx::sin(input_chunk);
      for (size_t j; j < T::size(); ++j) {
        EXPECT_EQ(result[j], stdResult[j]);
      }
    }
  }
}

TYPED_TEST(CopalTest_Templated, sin_taylor) {
  using mathlib = TypeParam;
  using T       = mathlib::T;
  using Float   = mathlib::Float;

  std::vector<Float> inputs = this->fixture_min_max();

  std::function<Float(Float)> stdSin;

  if constexpr (std::is_floating_point_v<T>) {
    for (size_t i; i < inputs.size(); ++i) {
      EXPECT_EQ(mathlib::sin_taylor(inputs[i]), std::sin(inputs[i]));
    }
  } else if constexpr (stdx::is_simd_v<T>) {
    for (size_t i; i < inputs.size(); i += T::size()) {
      T input_chunk(&inputs[i], stdx::element_aligned);
      T result = mathlib::sin_taylor(input_chunk);
      T stdResult = stdx::sin(input_chunk);
      for (size_t j; j < T::size(); ++j) {
        EXPECT_EQ(result[j], stdResult[j]);
      }
    }
  }
}