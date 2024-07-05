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

TYPED_TEST(CopalTest_Templated, fabs) {
  using mathlib = TypeParam;
  using T       = mathlib::T;
  using Float   = mathlib::Float;

  std::vector<Float> inputs = this->fixture_min_max();

  if constexpr (std::is_floating_point_v<T>) {
    for (size_t i; i < inputs.size(); ++i) {
      EXPECT_EQ(mathlib::fabs(inputs[i]), std::fabs(inputs[i]));
    }
  } else if constexpr (stdx::is_simd_v<T>) {
    for (size_t i; i < inputs.size(); i += T::size()) {
      T input_chunk(&inputs[i], stdx::element_aligned);
      T result = mathlib::fabs(input_chunk);
      for (size_t j; j < T::size(); ++j) {
        EXPECT_EQ(result[j], std::fabs(inputs[i+j]));
      }
    }
  }
}

TYPED_TEST(CopalTest_Templated, fmod) {
  using mathlib = TypeParam;
  using T       = mathlib::T;
  using Float   = mathlib::Float;

  std::vector<Float> inputs = this->fixture_min_max();

  if constexpr (std::is_floating_point_v<T>) {
    for (size_t i; i < inputs.size(); ++i) {
      for (size_t j = 0; j < inputs.size(); ++j) {
        T a = inputs[i];
        T b = inputs[j];

        T result   = mathlib::fmod(a, b);
        T baseline =     std::fmod(a, b);

        // two nans don't make a right
        if (!std::isnan(result) && !std::isnan(baseline))
          EXPECT_EQ(result, baseline);
      }
    }
  } else if constexpr (stdx::is_simd_v<T>) {
    for (size_t i; i < inputs.size(); ++i) {
      for (size_t j = 0; j < inputs.size(); j += T::size()) {
        T a_chunk(inputs[i]);
        T b_chunk(&inputs[j], stdx::element_aligned);
        T result = mathlib::fmod(a_chunk, b_chunk);
        for (size_t k = 0; k < T::size(); ++k) {
          Float baseline = std::fmod(a_chunk[k], b_chunk[k]);

          // two nans don't make a right
          if (!std::isnan(result[k]) && !std::isnan(baseline))
            EXPECT_EQ(result[k], baseline);
        }
      }
    }
  }
}

TYPED_TEST(CopalTest_Templated, lerp) {
  using mathlib = TypeParam;
  using T       = mathlib::T;
  using Float   = mathlib::Float;

  std::vector<Float> inputs = this->fixture_min_max();

   std::vector<std::pair<Float, Float>> lerpPairs = {
    {std::numeric_limits<Float>::min(), std::numeric_limits<Float>::max()},
    {std::numeric_limits<Float>::max(), std::numeric_limits<Float>::min()},
    {std::numeric_limits<Float>::min(), 0},
    {std::numeric_limits<Float>::max(), 0},
    {copal::num::pi_over_16<Float>, -copal::num::pi_over_16<Float>},
    {copal::num::pi_over_16<Float>, -copal::num::pi_over_16<Float>},
  };

  for (auto pair : lerpPairs) {
    auto [a, b] = pair;
    for (size_t i = 0; i < inputs.size(); ++i) {
      if constexpr (std::is_floating_point_v<T>) {
        for (size_t i; i < inputs.size(); ++i) {
          EXPECT_EQ(mathlib::lerp(a, b, inputs[i]), std::lerp(a, b, inputs[i]));
        }
      } else if constexpr (stdx::is_simd_v<T>) {
        for (size_t i; i < inputs.size(); i += T::size()) {
          T lerp(&inputs[i], stdx::element_aligned);
          T a_chunk(a);
          T b_chunk(b);
          T result = mathlib::lerp(a_chunk, b_chunk, lerp);
          for (size_t j; j < T::size(); ++j) {
            EXPECT_EQ(result[j], std::lerp(a, b, inputs[i]));
          }
        }
      }
    }
  }
}

TYPED_TEST(CopalTest_Templated, lerp_by_initity) {
  using mathlib = TypeParam;
  using T       = mathlib::T;
  using Float   = mathlib::Float;

  Float inf  = std::numeric_limits<Float>::infinity();
  Float ninf = -inf;

  for (auto val : {inf, ninf}) {
    std::vector<Float> expectedResult { val, val, val, inf, val, -val, ninf,  ninf};
    std::vector<Float> a              { 1,   val, 1,   val, val,  val,  val,  val };
    std::vector<Float> b              { 2,   2,   val, val, val,  val, -val,  2   };
    std::vector<Float> lerpAmount     { val, 1,   1  , 1  , val, -val,  1  , -val };

    if constexpr (std::is_floating_point_v<T>) {
      for (size_t i = 0; i < expectedResult.size(); ++i) {
        EXPECT_EQ(expectedResult[i] ,  mathlib::lerp(a[i], b[i], lerpAmount[i]));
      }
    } else if constexpr (stdx::is_simd_v<T>) {
      for (size_t i = 0; i < expectedResult.size(); i += T::size()) {
        T a_chunk   (&a[i],          stdx::element_aligned);
        T b_chunk   (&b[i],          stdx::element_aligned);
        T lerp_chunk(&lerpAmount[i], stdx::element_aligned);
        T result = mathlib::lerp(a_chunk, b_chunk, lerp_chunk);

        for (size_t j = i; j < T::size(); ++j) {
          EXPECT_EQ(expectedResult[j],  result[j]);
        }
      }
    }
  }
}

TYPED_TEST(CopalTest_Templated, floor) {
  using mathlib = TypeParam;
  using T       = mathlib::T;
  using Float   = mathlib::Float;

  std::vector<Float> inputs = this->fixture_min_max();

  if constexpr (std::is_floating_point_v<T>) {
    for (size_t i; i < inputs.size(); ++i) {
      EXPECT_EQ(mathlib::floor(inputs[i]), std::floor(inputs[i]));
    }
  } else if constexpr (stdx::is_simd_v<T>) {
    for (size_t i; i < inputs.size(); i += T::size()) {
      T input_chunk(&inputs[i], stdx::element_aligned);
      T result = mathlib::floor(input_chunk);
      for (size_t j = 0; j < T::size(); ++j) {
        EXPECT_EQ(result[j], std::floor(inputs[i+j]));
      }
    }
  }
}


TYPED_TEST(CopalTest_Templated, angle_normalization_is_symmetrical) {
  using mathlib = TypeParam;
  using T       = mathlib::T;
  using Float   = mathlib::Float;

  Float quarterCycle      = copal::num::pi_over_2<Float>;
  Float halfCycle         = copal::num::pi_x_1<Float>;
  Float fullCycle         = copal::num::pi_x_2<Float>;
  Float threeQuarterCycle = halfCycle + quarterCycle;

  alignas(128) std::vector<Float> inputs = this->create_fixture(0, quarterCycle);

  if constexpr (std::is_floating_point_v<T>) {
    for (auto input : inputs) {
      Float xIn_1 = input;
      Float xIn_2 = halfCycle - input;
      Float xIn_3 = halfCycle + input;
      Float xIn_4 = fullCycle - input;

      auto [x_1, sign_1] = mathlib::angle_normalization_pi_over_2(xIn_1);
      auto [x_2, sign_2] = mathlib::angle_normalization_pi_over_2(xIn_2);
      auto [x_3, sign_3] = mathlib::angle_normalization_pi_over_2(xIn_3);
      auto [x_4, sign_4] = mathlib::angle_normalization_pi_over_2(xIn_4);

      EXPECT_EQ(xIn_1, x_1);
      EXPECT_NEAR(xIn_1, x_2, tolerance<Float>::angle_normalization_is_symmetrical_x_2);
      EXPECT_NEAR(xIn_1, x_3, tolerance<Float>::angle_normalization_is_symmetrical_x_3);
      EXPECT_NEAR(xIn_1, x_4, tolerance<Float>::angle_normalization_is_symmetrical_x_4);
    }
  } else if constexpr (stdx::is_simd_v<T>) {
    size_t testSetsPerVector = T::size() / 4;

    for (size_t i = 0; i < inputs.size(); i += testSetsPerVector) {
      alignas(stdx::memory_alignment_v<T>)
        std::array<Float, T::size()> simdData {};

      // 1 block of 4 tests per iteration
      // float:  8 slots per register - 2 test blocks per vector
      // double: 4 slots per register - 1 test block  per vector
      for (size_t j = 0; j < testSetsPerVector; ++j) {
        const size_t k = j * testSetsPerVector;
        Float xIn = inputs[i+j];

        simdData[k + 0] = xIn;
        simdData[k + 1] = halfCycle - xIn;
        simdData[k + 2] = halfCycle + xIn;
        simdData[k + 3] = fullCycle - xIn;
      }

      T input_x_chunk(&simdData[0], stdx::element_aligned);
      auto [x, sign] = mathlib::angle_normalization_pi_over_2(input_x_chunk);

      for (size_t j = 0; j < testSetsPerVector; ++j) {
        Float xIn = inputs[i+j];
        const size_t k = j * testSetsPerVector;
        EXPECT_EQ(simdData[k], xIn);
        EXPECT_NEAR(x[k], x[k + 1], tolerance<Float>::simd_angle_normalization_is_symmetrical_x_2);
        EXPECT_NEAR(x[k], x[k + 2], tolerance<Float>::simd_angle_normalization_is_symmetrical_x_3);
        EXPECT_NEAR(x[k], x[k + 3], tolerance<Float>::simd_angle_normalization_is_symmetrical_x_4);
      }
    }
  }
}

TYPED_TEST(CopalTest_Templated, angle_normalization_sign_is_correct) {
  using mathlib = TypeParam;
  using T       = mathlib::T;
  using Float   = mathlib::Float;

  Float halfCycle = copal::num::pi_x_1<Float>;

  alignas(128) std::vector<Float> inputs
    = this->create_fixture(0, halfCycle, this->fixture_size);

  if constexpr (std::is_floating_point_v<T>) {
    for (auto input : inputs) {
      Float xIn1 = input;
      Float xIn2 = input + halfCycle;

      auto [x1, sign1] = mathlib::angle_normalization_pi_over_2(xIn1);
      auto [x2, sign2] = mathlib::angle_normalization_pi_over_2(xIn2);

      EXPECT_EQ(sign1,  1);
      EXPECT_EQ(sign2, -1);
    }
  } else if constexpr (stdx::is_simd_v<T>) {
    for (size_t i = 0; i < inputs.size(); i += T::size()) {
      T xChunk1 (&(inputs[i]), stdx::element_aligned);
      T xChunk2 {xChunk1 + halfCycle};

      auto [x1, sign1] = mathlib::angle_normalization_pi_over_2(xChunk1);
      auto [x2, sign2] = mathlib::angle_normalization_pi_over_2(xChunk2);

      for (size_t j = 0; j < T::size(); ++j) {
        EXPECT_EQ(sign1[j],  1);
        EXPECT_EQ(sign2[j], -1);
      }
    }
  }
}

TYPED_TEST(CopalTest_Templated, angle_normalization_x_always_positive) {
  using mathlib = TypeParam;
  using T       = mathlib::T;
  using Float   = mathlib::Float;

  alignas(128) std::vector<Float> inputs = this->fixture_min_max();

  if constexpr (std::is_floating_point_v<T>) {
    for (auto input : inputs) {
      auto [x, sign] = mathlib::angle_normalization_pi_over_2(input);

      EXPECT_GE(x, 0);
    }
  } else if constexpr (stdx::is_simd_v<T>) {
    for (size_t i = 0; i < inputs.size(); i += T::size()) {
      T xChunk (&(inputs[i]), stdx::element_aligned);
      auto [x, sign] = mathlib::angle_normalization_pi_over_2(xChunk);

      for (size_t j = 0; j < T::size(); ++j) {
        EXPECT_GE(x[j], 0);
      }
    }
  }
}

TYPED_TEST(CopalTest_Templated, angle_normalization_is_periodic) {
  using mathlib = TypeParam;
  using T       = mathlib::T;
  using Float   = mathlib::Float;

  Float fullCycle = copal::num::pi_x_2<Float>;

  alignas(128) std::vector<Float> inputs
    = this->create_fixture(0, std::nextafter(fullCycle, 0), this->fixture_size);

  std::ranges::iota_view<int, int> multipliers {1, 10};

  if constexpr (std::is_floating_point_v<T>) {
    for (auto input : inputs) {
      auto [x1, sign1] = mathlib::angle_normalization_pi_over_2(input);
      for (auto multiplier : multipliers) {
        Float offset = multiplier * fullCycle;
        auto [x2, sign2] = mathlib::angle_normalization_pi_over_2(input + offset);

        EXPECT_NEAR(x1,  x2, tolerance<Float>::angle_normalization_is_periodic);

        Float absError      = std::abs(x1 - x2);
        Float zeroProximity = std::abs(0  - x2);
        if (zeroProximity > tolerance<Float>::angle_norm_periodic_when_close
            && absError   > tolerance<Float>::angle_norm_periodic_when_close
        ) EXPECT_EQ(sign1,  sign2);
      }
    }
  } else if constexpr (stdx::is_simd_v<T>) {
    for (size_t i = 0; i < inputs.size(); i += T::size()) {
      T xChunk1 (&(inputs[i]), stdx::element_aligned);

      auto [x1, sign1] = mathlib::angle_normalization_pi_over_2(xChunk1);
      for (auto multiplier : multipliers) {
        T xChunk2 { xChunk1 + (fullCycle * multiplier) };

        auto [x2, sign2] = mathlib::angle_normalization_pi_over_2(xChunk2);

        for (size_t j = 0; j < T::size(); ++j) {
          EXPECT_NEAR(x1[j], x2[j], tolerance<Float>::angle_normalization_is_periodic);

          Float absError      = std::abs(x1[j] - x2[j]);
          Float zeroProximity = std::abs(0     - x2[j]);
          if (zeroProximity > tolerance<Float>::angle_norm_periodic_when_close
              && absError   > tolerance<Float>::angle_norm_periodic_when_close
          ) EXPECT_EQ(sign1[j],  sign2[j]);
        }
      }
    }
  }
}
