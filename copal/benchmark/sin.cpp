#include <array>
#include <concepts>
#include <experimental/simd>
#include <experimental/bits/simd.h>
#include <type_traits>
#include <random>

// ----
// libs
#include "benchmark/benchmark.h"

// ----
// headers
#include "math_concept.hpp"
#include "num.hpp"
#include "sin_lut.hpp"
#include "stdx_definition.hpp"


// ----
// fixtures
const size_t testInputSize = 2048;

template<std::floating_point T, std::size_t size>
std::array<T, size> randArray(T lowest, T highest) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<T> dist(lowest, highest);

    std::array<T, size> randomArray;
    for (auto& element : randomArray) {
        element = dist(gen);
    }
    return randomArray;
}

#define aligned_array(T, size) \
alignas(stdx::memory_alignment_v<stdx::native_simd<T>>) std::array<T, size>

// --------------------------------
// benchmarks : non-simd

template<std::floating_point T, int numerator, int denominator, T op(T)>
struct bm_single_sin_params {
    using Type = T;
    constexpr static T max = T(numerator) / T(denominator) * copal::num::pi_x_1<T>;
    constexpr static T min = -max;
    static T exec(T x) { return op(x); } 
};

template<typename params>
static void BM_copal_single_sin_v2(benchmark::State& state) {
  using T = params::Type;
  aligned_array(T, testInputSize) x_in =
    randArray<T, testInputSize>(params::min, params::max);

  for (auto _ : state) {
    for(size_t i = 0; i < testInputSize; ++i) {
      benchmark::DoNotOptimize(params::exec(x_in[i]));
      benchmark::ClobberMemory();
    }
  }
}

BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  1, 4, copal::Scalar<float>::sin_lookup>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  1, 2, copal::Scalar<float>::sin_lookup>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  1, 1, copal::Scalar<float>::sin_lookup>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  2, 1, copal::Scalar<float>::sin_lookup>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  4, 1, copal::Scalar<float>::sin_lookup>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  8, 1, copal::Scalar<float>::sin_lookup>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float, 16, 1, copal::Scalar<float>::sin_lookup>>);

BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  1, 4, copal::Scalar<float>::sin_taylor>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  1, 2, copal::Scalar<float>::sin_taylor>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  1, 1, copal::Scalar<float>::sin_taylor>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  2, 1, copal::Scalar<float>::sin_taylor>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  4, 1, copal::Scalar<float>::sin_taylor>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  8, 1, copal::Scalar<float>::sin_taylor>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float, 16, 1, copal::Scalar<float>::sin_taylor>>);

BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  1, 4, copal::Stdlib<float>::sin_lookup>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  1, 2, copal::Stdlib<float>::sin_lookup>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  1, 1, copal::Stdlib<float>::sin_lookup>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  2, 1, copal::Stdlib<float>::sin_lookup>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  4, 1, copal::Stdlib<float>::sin_lookup>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  8, 1, copal::Stdlib<float>::sin_lookup>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float, 16, 1, copal::Stdlib<float>::sin_lookup>>);

BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  1, 4, copal::Stdlib<float>::sin_taylor>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  1, 2, copal::Stdlib<float>::sin_taylor>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  1, 1, copal::Stdlib<float>::sin_taylor>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  2, 1, copal::Stdlib<float>::sin_taylor>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  4, 1, copal::Stdlib<float>::sin_taylor>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  8, 1, copal::Stdlib<float>::sin_taylor>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float, 16, 1, copal::Stdlib<float>::sin_taylor>>);

BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  1, 4, copal::Stdlib<float>::sin_stdlib>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  1, 2, copal::Stdlib<float>::sin_stdlib>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  1, 1, copal::Stdlib<float>::sin_stdlib>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  2, 1, copal::Stdlib<float>::sin_stdlib>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  4, 1, copal::Stdlib<float>::sin_stdlib>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float,  8, 1, copal::Stdlib<float>::sin_stdlib>>);
BENCHMARK(BM_copal_single_sin_v2<bm_single_sin_params<float, 16, 1, copal::Stdlib<float>::sin_stdlib>>);

// --------------------------------
// benchmarks : simd

template<std::floating_point T, int numerator, int denominator, stdx::native_simd<T> op(stdx::native_simd<T>)>
struct bm_simd_sin_params {
    using Type = T;
    constexpr static T max = T(numerator) / T(denominator) * copal::num::pi_x_1<T>;
    constexpr static T min = -max;
    static stdx::native_simd<T> exec(stdx::native_simd<T> x) { return op(x); } 
};

template<typename params>
static void BM_copal_simd_sin_v2(benchmark::State& state) {
  using T = params::Type;
  aligned_array(T, testInputSize) x_in =
    randArray<T, testInputSize>(params::min, params::max);

  aligned_array(T, stdx::native_simd<T>::size()) x_out = {};
  for (auto _ : state) {
    for (size_t i = 0; i < x_in.size(); i += stdx::native_simd<T>::size()) {
      stdx::native_simd<T> simd_x(&x_in[i], stdx::vector_aligned);
      const std::size_t size = stdx::native_simd<T>::size();
        size_t idx = i / size;
        benchmark::DoNotOptimize(params::exec(simd_x));
        benchmark::ClobberMemory();
    }
  }
}

BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  1, 4, copal::VectorImpl<float>::sin_lookup>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  1, 2, copal::VectorImpl<float>::sin_lookup>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  1, 1, copal::VectorImpl<float>::sin_lookup>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  2, 1, copal::VectorImpl<float>::sin_lookup>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  4, 1, copal::VectorImpl<float>::sin_lookup>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  8, 1, copal::VectorImpl<float>::sin_lookup>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float, 16, 1, copal::VectorImpl<float>::sin_lookup>>);

BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  1, 4, copal::VectorImpl<float>::sin_taylor>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  1, 2, copal::VectorImpl<float>::sin_taylor>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  1, 1, copal::VectorImpl<float>::sin_taylor>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  2, 1, copal::VectorImpl<float>::sin_taylor>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  4, 1, copal::VectorImpl<float>::sin_taylor>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  8, 1, copal::VectorImpl<float>::sin_taylor>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float, 16, 1, copal::VectorImpl<float>::sin_taylor>>);

BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  1, 4, copal::VectorStdx<float>::sin_lookup>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  1, 2, copal::VectorStdx<float>::sin_lookup>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  1, 1, copal::VectorStdx<float>::sin_lookup>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  2, 1, copal::VectorStdx<float>::sin_lookup>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  4, 1, copal::VectorStdx<float>::sin_lookup>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  8, 1, copal::VectorStdx<float>::sin_lookup>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float, 16, 1, copal::VectorStdx<float>::sin_lookup>>);

BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  1, 4, copal::VectorStdx<float>::sin_taylor>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  1, 2, copal::VectorStdx<float>::sin_taylor>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  1, 1, copal::VectorStdx<float>::sin_taylor>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  2, 1, copal::VectorStdx<float>::sin_taylor>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  4, 1, copal::VectorStdx<float>::sin_taylor>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  8, 1, copal::VectorStdx<float>::sin_taylor>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float, 16, 1, copal::VectorStdx<float>::sin_taylor>>);

BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  1, 4, copal::VectorStdx<float>::sin_stdlib>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  1, 2, copal::VectorStdx<float>::sin_stdlib>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  1, 1, copal::VectorStdx<float>::sin_stdlib>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  2, 1, copal::VectorStdx<float>::sin_stdlib>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  4, 1, copal::VectorStdx<float>::sin_stdlib>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float,  8, 1, copal::VectorStdx<float>::sin_stdlib>>);
BENCHMARK(BM_copal_simd_sin_v2<bm_simd_sin_params<float, 16, 1, copal::VectorStdx<float>::sin_stdlib>>);




#define asdfasdf(bm_func, type_params, type_float, impl_class, impl_method)\
BENCHMARK(bm_func<type_params<type_float,  1, 4, impl_class<type_float>::impl_method>>);\
BENCHMARK(bm_func<type_params<type_float,  1, 2, impl_class<type_float>::impl_method>>);\
BENCHMARK(bm_func<type_params<type_float,  1, 1, impl_class<type_float>::impl_method>>);\
BENCHMARK(bm_func<type_params<type_float,  2, 1, impl_class<type_float>::impl_method>>);\
BENCHMARK(bm_func<type_params<type_float,  4, 1, impl_class<type_float>::impl_method>>);\
BENCHMARK(bm_func<type_params<type_float,  8, 1, impl_class<type_float>::impl_method>>);\
BENCHMARK(bm_func<type_params<type_float, 16, 1, impl_class<type_float>::impl_method>>);\

asdfasdf(BM_copal_single_sin_v2, bm_single_sin_params, float, copal::Scalar, sin_lookup)