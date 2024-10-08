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
#include "copal/vector_impl.hpp"
#include "copal/vector_stdx.hpp"
#include "copal/scalar.hpp"
#include "copal/stdlib.hpp"
#include "copal/stdx_definition.hpp"


// ----
// fixtures
const size_t testInputSize = 1000000;

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

// --------------------------------
// benchmark : non-simd

template<std::floating_point T, int multiplier, T op(T)>
struct bm_scalar_sin_params {
    using Type = T;
    constexpr static T max = T(multiplier) * copal::num::pi_x_1<T>;
    constexpr static T min = -max;
    static T exec(T x) { return op(x); } 
};

template<typename params>
static void BM_sin_scalar(benchmark::State& state) {
  using T = params::Type;
  std::array<T, testInputSize> x_in =
    randArray<T, testInputSize>(params::min, params::max);

  for (auto _ : state) {
    for (size_t i = 0; i < testInputSize; ++i) {
      benchmark::DoNotOptimize(params::exec(x_in[i]));
      benchmark::ClobberMemory();
    }
  }
}

// --------------------------------
// benchmarks : simd

template<std::floating_point T, int multiplier, stdx::native_simd<T> op(stdx::native_simd<T>)>
struct bm_simd_sin_params {
    using Type = T;
    constexpr static T max = T(multiplier) * copal::num::pi_x_1<T>;
    constexpr static T min = -max;

    static stdx::native_simd<T> exec(const stdx::native_simd<T>& x) {return op(x);}
};

template<typename params>
static void BM_sin_simd(benchmark::State& state) {
  using T = typename params::Type;
  alignas(stdx::memory_alignment_v<stdx::native_simd<T>>)
    std::array<T, testInputSize> x_in =
        randArray<T, testInputSize>(params::min, params::max);

  for (auto _ : state) {
    for (size_t i = 0; i < x_in.size(); i += stdx::native_simd<T>::size()) {
      stdx::native_simd<T> simd_x(&x_in[i], stdx::element_aligned);

      benchmark::DoNotOptimize(params::exec(simd_x));
      benchmark::ClobberMemory();
    }
  }
}

#define bm_declare_set(bm_func, type_params, type_float, impl_class, impl_method)\
BENCHMARK(bm_func<type_params<type_float,   2, impl_class<type_float>::impl_method>>);\
BENCHMARK(bm_func<type_params<type_float,   4, impl_class<type_float>::impl_method>>);\
BENCHMARK(bm_func<type_params<type_float,  40, impl_class<type_float>::impl_method>>);\
BENCHMARK(bm_func<type_params<type_float, 400, impl_class<type_float>::impl_method>>);

bm_declare_set(BM_sin_scalar, bm_scalar_sin_params, float, copal::Scalar,     sin_lookup);
bm_declare_set(BM_sin_scalar, bm_scalar_sin_params, float, copal::Scalar,     sin_taylor)
bm_declare_set(BM_sin_scalar, bm_scalar_sin_params, float, copal::Stdlib,     sin_lookup)
bm_declare_set(BM_sin_scalar, bm_scalar_sin_params, float, copal::Stdlib,     sin_taylor)
bm_declare_set(BM_sin_scalar, bm_scalar_sin_params, float, copal::Stdlib,     sin_stdlib)

bm_declare_set(BM_sin_simd,   bm_simd_sin_params,   float, copal::VectorImpl, sin_lookup);
bm_declare_set(BM_sin_simd,   bm_simd_sin_params,   float, copal::VectorImpl, sin_taylor)
bm_declare_set(BM_sin_simd,   bm_simd_sin_params,   float, copal::VectorStdx, sin_lookup)
bm_declare_set(BM_sin_simd,   bm_simd_sin_params,   float, copal::VectorStdx, sin_taylor)
bm_declare_set(BM_sin_simd,   bm_simd_sin_params,   float, copal::VectorStdx, sin_stdlib)

bm_declare_set(BM_sin_scalar, bm_scalar_sin_params, double, copal::Scalar,     sin_lookup);
bm_declare_set(BM_sin_scalar, bm_scalar_sin_params, double, copal::Scalar,     sin_taylor)
bm_declare_set(BM_sin_scalar, bm_scalar_sin_params, double, copal::Stdlib,     sin_lookup)
bm_declare_set(BM_sin_scalar, bm_scalar_sin_params, double, copal::Stdlib,     sin_taylor)
bm_declare_set(BM_sin_scalar, bm_scalar_sin_params, double, copal::Stdlib,     sin_stdlib)

bm_declare_set(BM_sin_simd,   bm_simd_sin_params,   double, copal::VectorImpl, sin_lookup);
bm_declare_set(BM_sin_simd,   bm_simd_sin_params,   double, copal::VectorImpl, sin_taylor)
bm_declare_set(BM_sin_simd,   bm_simd_sin_params,   double, copal::VectorStdx, sin_lookup)
bm_declare_set(BM_sin_simd,   bm_simd_sin_params,   double, copal::VectorStdx, sin_taylor)
bm_declare_set(BM_sin_simd,   bm_simd_sin_params,   double, copal::VectorStdx, sin_stdlib)