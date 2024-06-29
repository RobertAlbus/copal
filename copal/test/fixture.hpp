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
  void SetUp() override {
    T min = std::numeric_limits<T>::min();
    T max = std::numeric_limits<T>::max();

    fillVecLerp(testValuesA,  fixture_size, min, max);
    fillVecLerp(testValuesB,  fixture_size, min, max);
    fillVecLerp(quarterCycle, fixture_size + 1 /*up to but not including*/, 0, copal::num::pi_over_2<T>);
    fillVecLerp(halfCycle,    fixture_size + 1 /*up to but not including*/, 0, copal::num::pi_x_1<T>);
    fillVecLerp(fullCycle,    fixture_size + 1 /*up to but not including*/, 0, copal::num::pi_x_2<T>);
  }

  const size_t fixture_size = 4096;
  std::vector<T> testValuesA;
  std::vector<T> testValuesB;

  std::vector<T> quarterCycle;
  std::vector<T> halfCycle;
  std::vector<T> fullCycle;
private:

  void fillVecLerp(std::vector<T>& vec, size_t size, T min, T max) {
    vec.reserve(fixture_size);
    for (size_t i = 0; i <= fixture_size; ++i) {
      T lerpAmount = T(i) / size;
      vec.emplace_back(std::lerp(min, max, lerpAmount));
    }
  }

};

using fpm = ::testing::Types<float, double>;
TYPED_TEST_SUITE(CopalTest, fpm);