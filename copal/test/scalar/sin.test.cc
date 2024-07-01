#include <cmath>
#include <concepts>
#include <cstdlib>
#include <format>
#include <limits>
#include <numbers>
#include <type_traits>
#include <typeinfo>
#include <utility>

#include <gtest/gtest.h>

#include "fixture.hpp"
#include "scalar/sin.hpp"
#include "stdlib/sin.hpp"


TYPED_TEST(CopalTest, sin_lookup) {
  using T = TypeParam;
  std::vector<T> inputs = this->create_fixture(-6.5, 6.5, 30);

  for (auto input : inputs) {
    T sinScalar = copal::scalar::sin_lookup(input);
    T stdsin = std::sin(input);

    T absError = std::abs(sinScalar - stdsin);

    if constexpr (std::is_same_v<T,float>) {
        EXPECT_NEAR(sinScalar, stdsin, tolerance<T>::sin_lookup_float);
    } else if constexpr (std::is_same_v<T,double>) {
        EXPECT_NEAR(sinScalar, stdsin, tolerance<T>::sin_lookup_double);
    } else {
        static_assert(false, "CopalTest::sin_lookup must be run on single or double precision float");
    }
  }
}

TYPED_TEST(CopalTest, sin_taylor) {
  using T = TypeParam;
  std::vector<T> inputs = this->create_fixture(-6.5, 6.5, 30);

  for (auto input : inputs) {
    T sinScalar = copal::scalar::sin_taylor(input);
    T stdsin = std::sin(input);

    T absError = std::abs(sinScalar - stdsin);

    if constexpr (std::is_same_v<T,float>) {
        EXPECT_NEAR(sinScalar, stdsin, tolerance<T>::sin_taylor_float);
    } else if constexpr (std::is_same_v<T,double>) {
        EXPECT_NEAR(sinScalar, stdsin, tolerance<T>::sin_taylor_double);
    } else {
        static_assert(false, "CopalTest::sin_taylor must be run on single or double precision float");
    }
  }
}
