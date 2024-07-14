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

  if constexpr (std::is_floating_point_v<T>) {
    for (size_t i = 0; i < inputs.size(); ++i) {
      Float a = mathlib::sin_lookup(inputs[i]);
      Float b = std::sin(inputs[i]);
      EXPECT_NEAR(a, b, tolerance<Float>::sin_lookup);
    }
  } else if constexpr (stdx::is_simd_v<T>) {
    for (size_t i = 0; i < inputs.size(); i += T::size()) {
      T input_chunk(&inputs[i], stdx::element_aligned);
      T result = mathlib::sin_lookup(input_chunk);
      T stdResult = stdx::sin(input_chunk);
      for (size_t j = 0; j < T::size(); ++j) {
        Float a = result[j];
        Float b = stdResult[j];
        EXPECT_NEAR(a, b, tolerance<Float>::sin_lookup_simd);
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
    for (size_t i = 0; i < inputs.size(); ++i) {
      Float a = mathlib::sin_taylor(inputs[i]);
      Float b = std::sin(inputs[i]);

      // EXPECT_<T>_EQ is true within 4 ULP
      // line 286 in gtest/internal/gtest-internal.h
      EXPECT_DOUBLE_EQ(a, b);

      // bit-for-bit comparison
      size_t mem1 = 0, mem2 = 0;
      std::memcpy(&mem1, &a, sizeof(Float));
      std::memcpy(&mem2, &b, sizeof(Float));
      EXPECT_TRUE(mem1 == mem2);
    }
  } else if constexpr (stdx::is_simd_v<T>) {
    for (size_t i = 0; i < inputs.size(); i += T::size()) {
      T input_chunk(&inputs[i], stdx::element_aligned);
      T result = mathlib::sin_taylor(input_chunk);
      T stdResult = stdx::sin(input_chunk);
      for (size_t j = 0; j < T::size(); ++j) {
        Float a = result[j];
        Float b = stdResult[j];

        // EXPECT_<T>_EQ is true within 4 ULP
        // line 286 in gtest/internal/gtest-internal.h
        EXPECT_DOUBLE_EQ(a, b);

        // bit-for-bit comparison
        size_t mem1 = 0, mem2 = 0;
        std::memcpy(&mem1, &a, sizeof(Float));
        std::memcpy(&mem2, &b, sizeof(Float));
        EXPECT_TRUE(mem1 == mem2);
      }
    }
  }
}