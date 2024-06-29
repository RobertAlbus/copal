#include <algorithm>
#include <cmath>
#include <concepts>
#include <format>
#include <limits>
#include <numbers>
#include <type_traits>
#include <utility>

#include <gtest/gtest.h>

#include "fixture.hpp"
#include "scalar/math.hpp"
#include "num.hpp"
#include "stdlib/math.hpp"

template <std::floating_point T>
struct tolerance {
  static constexpr T fmod = 50 * std::numeric_limits<T>::epsilon();
  static constexpr T angle_normalization_is_symmetrical_x_2 = 1.193e-7;
  static constexpr T angle_normalization_is_symmetrical_x_3 = 2.385e-7;
  static constexpr T angle_normalization_is_symmetrical_x_4 = 2.385e-7;
  static constexpr T angle_normalization_is_periodic        = 6.915e-6;
};

TYPED_TEST(CopalTest, fabs) {
  using T = TypeParam;

  for (auto x : this->fixture_min_max()) {
    EXPECT_FLOAT_EQ(
      copal::scalar::fabs(x),
      copal::stdlib::fabs(x));
  }
}

TYPED_TEST(CopalTest, fmod) {
  using T = TypeParam;
  std::vector<T> inputsA = this->fixture_min_max();
  std::vector<T> inputsB; inputsB.resize(inputsA.size()); 
  std::reverse_copy(inputsA.begin(), inputsA.end(), inputsB.begin());

  for (size_t i; i < this->fixture_size; ++i) {
    T fmodCopal = copal::scalar::fmod(inputsA[i], inputsB[i]);
    T fmodStd   = copal::stdlib::fmod(inputsA[i], inputsB[i]);

    // non-std copal implementations return [0..|b|] like arithmetic abs
    // normalize the stdlib version into a positive number
    if (fmodStd < 0) fmodStd += inputsB[i];
    EXPECT_FLOAT_EQ(fmodCopal, fmodStd);
  }
}

TYPED_TEST(CopalTest, lerp) {
  using T = TypeParam;

  std::vector<std::pair<T, T>> lerpPairs = {
    {std::numeric_limits<T>::min(), std::numeric_limits<T>::max()},
    {std::numeric_limits<T>::max(), std::numeric_limits<T>::min()},
    {std::numeric_limits<T>::min(), 0},
    {std::numeric_limits<T>::max(), 0},
    {copal::num::pi_over_16<T>, -copal::num::pi_over_16<T>},
    {copal::num::pi_over_16<T>, -copal::num::pi_over_16<T>},
  };

  for (auto pair : lerpPairs) {
    auto [a, b] = pair;
    for (auto lerpValue : this->fixture_min_max()) {
      T lerpScalar = copal::scalar::lerp<T>(a, b, lerpValue);
      T lerpStdlib = copal::stdlib::lerp<T>(a, b, lerpValue);

      EXPECT_FLOAT_EQ(lerpScalar, lerpStdlib);
    }
  }
}

TYPED_TEST(CopalTest, lerp_by_initity) {
  using T = TypeParam;

  for (auto lerpValue : this->fixture_min_max()) {
    T inf  = std::numeric_limits<T>::infinity();
    T ninf = -inf;

    for (auto val : {inf, ninf}) {
      EXPECT_EQ(val,  copal::scalar::lerp<T>( 1,    2,    val ));
      EXPECT_EQ(val,  copal::scalar::lerp<T>( val,  2,    1   ));
      EXPECT_EQ(val,  copal::scalar::lerp<T>( 1,    val,  1   ));
      EXPECT_EQ(inf,  copal::scalar::lerp<T>( val,  val,  1   ));
      EXPECT_EQ(val,  copal::scalar::lerp<T>( val,  val,  val ));
      EXPECT_EQ(-val, copal::scalar::lerp<T>( val,  val, -val ));
      EXPECT_EQ(ninf, copal::scalar::lerp<T>( val, -val,  1   ));
      EXPECT_EQ(ninf, copal::scalar::lerp<T>( val,  2,   -val ));
    }
  }
}

TYPED_TEST(CopalTest, floor) {
  using T = TypeParam;

  for (auto input : this->fixture_min_max()) {
    T floorScalar = copal::scalar::floor(input);
    T floorStdlib = copal::stdlib::floor(input);
    
    EXPECT_FLOAT_EQ(floorScalar, floorStdlib);
  }
}

TYPED_TEST(CopalTest, angle_normalization_is_symmetrical) {
  using T = TypeParam;

  T quarterCycle      = copal::num::pi_over_2<T>;
  T halfCycle         = copal::num::pi_x_1<T>;
  T fullCycle         = copal::num::pi_x_2<T>;
  T threeQuarterCycle = halfCycle + quarterCycle;

  for (auto input : this->fixture_quarter_cycle()) {
    T xIn_1 = input;
    T xIn_2 = halfCycle - input;
    T xIn_3 = input + halfCycle;
    T xIn_4 = fullCycle - input;

    auto [x_1, sign_1] = copal::scalar::angle_normalization_pi_over_2<T>(xIn_1);
    auto [x_2, sign_2] = copal::scalar::angle_normalization_pi_over_2<T>(xIn_2);
    auto [x_3, sign_3] = copal::scalar::angle_normalization_pi_over_2<T>(xIn_3);
    auto [x_4, sign_4] = copal::scalar::angle_normalization_pi_over_2<T>(xIn_4);
    
    EXPECT_FLOAT_EQ(xIn_1, x_1);
    EXPECT_NEAR(xIn_1, x_2, tolerance<T>::angle_normalization_is_symmetrical_x_2);
    EXPECT_NEAR(xIn_1, x_3, tolerance<T>::angle_normalization_is_symmetrical_x_3);
    EXPECT_NEAR(xIn_1, x_4, tolerance<T>::angle_normalization_is_symmetrical_x_4);
  }
}

TYPED_TEST(CopalTest, angle_normalization_sign_is_correct) {
  using T = TypeParam;
  for (auto input : this->fixture_half_cycle()) {
    T xIn1 = input;
    T xIn2 = input + copal::num::pi_x_1<T>;

    auto [x1, sign1] = copal::scalar::angle_normalization_pi_over_2<T>(xIn1);
    auto [x2, sign2] = copal::scalar::angle_normalization_pi_over_2<T>(xIn2);
    
    EXPECT_EQ(sign1,  1);
    EXPECT_EQ(sign2, -1);
  }
}

TYPED_TEST(CopalTest, angle_normalization_is_periodic) {
  using T = TypeParam;
  std::vector<T> multipliers {1, 2, 3, 4, 10};

  T oneCycle= copal::num::pi_x_2<T>;
  for (auto input : this->fixture_full_cycle()) {
    auto [x1, sign1] = copal::scalar::angle_normalization_pi_over_2<T>(input);
    for (auto multiplier : multipliers) {
      auto [x2, sign2] = copal::scalar::angle_normalization_pi_over_2<T>(input + (multiplier * oneCycle));
      
      EXPECT_NEAR(x1,  x2, tolerance<T>::angle_normalization_is_periodic);
      EXPECT_FLOAT_EQ(sign1,  sign2);
    }
  }
}
