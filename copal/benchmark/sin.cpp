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
#include "scalar/sin.hpp"
#include "num.hpp"
#include "sin_lut.hpp"
#include "stdx_definition.hpp"
#include "stdlib/sin.hpp"
#include "vector_impl/sin.hpp"
#include "vector_stdx/sin.hpp"



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

template<std::floating_point T, int numerator, int denominator>
struct pi_params {
    using Type = T;
    constexpr static T max = T(numerator) / T(denominator) * copal::num::pi_x_1<T>;
    constexpr static T min = -max;
};                                                                               


#define BM_copal_single(impl_name, impl)\
template<typename params>                                   \
static void BM_copal_##impl_name(benchmark::State& state) { \
  using T = params::Type;                                   \
  aligned_array(T, testInputSize) x_in =                    \
    randArray<T, testInputSize>(params::min, params::max);  \
                                                            \
  for (auto _ : state) {                                    \
    for(size_t i = 0; i < testInputSize; ++i) {             \
      benchmark::DoNotOptimize(impl(x_in[i]));              \
      benchmark::ClobberMemory();                           \
    }                                                       \
  }                                                         \
}

BM_copal_single(scalar_sin_lookup, copal::scalar::sin_lookup)
BM_copal_single(scalar_sin_taylor, copal::scalar::sin_taylor)
BM_copal_single(stdlib_sin_lookup, copal::stdlib::sin_lookup)
BM_copal_single(stdlib_sin_taylor, copal::stdlib::sin_taylor)
BM_copal_single(stdlib_sin_stdlib, copal::stdlib::sin_stdlib)

BENCHMARK(BM_copal_scalar_sin_lookup<pi_params<float,  1, 4>>);
BENCHMARK(BM_copal_scalar_sin_lookup<pi_params<float,  1, 2>>);
BENCHMARK(BM_copal_scalar_sin_lookup<pi_params<float,  1, 1>>);
BENCHMARK(BM_copal_scalar_sin_lookup<pi_params<float,  2, 1>>);
BENCHMARK(BM_copal_scalar_sin_lookup<pi_params<float,  4, 1>>);
BENCHMARK(BM_copal_scalar_sin_lookup<pi_params<float,  8, 1>>);
BENCHMARK(BM_copal_scalar_sin_lookup<pi_params<float, 16, 1>>);

BENCHMARK(BM_copal_scalar_sin_taylor<pi_params<float,  1, 4>>);
BENCHMARK(BM_copal_scalar_sin_taylor<pi_params<float,  1, 2>>);
BENCHMARK(BM_copal_scalar_sin_taylor<pi_params<float,  1, 1>>);
BENCHMARK(BM_copal_scalar_sin_taylor<pi_params<float,  2, 1>>);
BENCHMARK(BM_copal_scalar_sin_taylor<pi_params<float,  4, 1>>);
BENCHMARK(BM_copal_scalar_sin_taylor<pi_params<float,  8, 1>>);
BENCHMARK(BM_copal_scalar_sin_taylor<pi_params<float, 16, 1>>);

BENCHMARK(BM_copal_stdlib_sin_lookup<pi_params<float,  1, 4>>);
BENCHMARK(BM_copal_stdlib_sin_lookup<pi_params<float,  1, 2>>);
BENCHMARK(BM_copal_stdlib_sin_lookup<pi_params<float,  1, 1>>);
BENCHMARK(BM_copal_stdlib_sin_lookup<pi_params<float,  2, 1>>);
BENCHMARK(BM_copal_stdlib_sin_lookup<pi_params<float,  4, 1>>);
BENCHMARK(BM_copal_stdlib_sin_lookup<pi_params<float,  8, 1>>);
BENCHMARK(BM_copal_stdlib_sin_lookup<pi_params<float, 16, 1>>);

BENCHMARK(BM_copal_stdlib_sin_taylor<pi_params<float,  1, 4>>);
BENCHMARK(BM_copal_stdlib_sin_taylor<pi_params<float,  1, 2>>);
BENCHMARK(BM_copal_stdlib_sin_taylor<pi_params<float,  1, 1>>);
BENCHMARK(BM_copal_stdlib_sin_taylor<pi_params<float,  2, 1>>);
BENCHMARK(BM_copal_stdlib_sin_taylor<pi_params<float,  4, 1>>);
BENCHMARK(BM_copal_stdlib_sin_taylor<pi_params<float,  8, 1>>);
BENCHMARK(BM_copal_stdlib_sin_taylor<pi_params<float, 16, 1>>);

BENCHMARK(BM_copal_stdlib_sin_stdlib<pi_params<float,  1, 4>>);
BENCHMARK(BM_copal_stdlib_sin_stdlib<pi_params<float,  1, 2>>);
BENCHMARK(BM_copal_stdlib_sin_stdlib<pi_params<float,  1, 1>>);
BENCHMARK(BM_copal_stdlib_sin_stdlib<pi_params<float,  2, 1>>);
BENCHMARK(BM_copal_stdlib_sin_stdlib<pi_params<float,  4, 1>>);
BENCHMARK(BM_copal_stdlib_sin_stdlib<pi_params<float,  8, 1>>);
BENCHMARK(BM_copal_stdlib_sin_stdlib<pi_params<float, 16, 1>>);

// --------------------------------
// benchmarks : simd

#define BM_define_copal_simd(impl_name, impl)                    \
template<typename params>                                        \
static void BM_copal_##impl_name(benchmark::State& state) {      \
  using T = params::Type;                                        \
  aligned_array(T, testInputSize) x_in =                         \
          randArray<T, testInputSize>(params::min, params::max); \
                                                                 \
  aligned_array(T, stdx::native_simd<T>::size()) x_out = {};     \
  for (auto _ : state) {                                         \
    stdx::native_simd<T> simd_x;                                 \
    simd_x.copy_from(&x_in[0], stdx::vector_aligned);            \
    stdx::native_simd<T> simd_x_out;                             \
    const std::size_t size = stdx::native_simd<T>::size();       \
    for(std::size_t i = 0; i + size <= x_in.size(); i += size) { \
      size_t idx = i / size;                                     \
      stdx::native_simd<T> chunk_x = simd_x[idx];                \
      benchmark::DoNotOptimize(simd_x_out = impl(simd_x));       \
      simd_x_out.copy_to(&(x_out[0]), stdx::vector_aligned);     \
      benchmark::ClobberMemory();                                \
    }                                                            \
  }                                                              \
}

BM_define_copal_simd(vector_impl_sin_lookup, copal::vector_impl::sin_lookup)
BM_define_copal_simd(vector_impl_sin_taylor, copal::vector_impl::sin_taylor)
BM_define_copal_simd(vector_stdx_sin_lookup, copal::vector_stdx::sin_lookup)
BM_define_copal_simd(vector_stdx_sin_taylor, copal::vector_stdx::sin_taylor)

BENCHMARK(BM_copal_vector_impl_sin_lookup<pi_params<float,  1, 4>>);
BENCHMARK(BM_copal_vector_impl_sin_lookup<pi_params<float,  1, 2>>);
BENCHMARK(BM_copal_vector_impl_sin_lookup<pi_params<float,  1, 1>>);
BENCHMARK(BM_copal_vector_impl_sin_lookup<pi_params<float,  2, 1>>);
BENCHMARK(BM_copal_vector_impl_sin_lookup<pi_params<float,  4, 1>>);
BENCHMARK(BM_copal_vector_impl_sin_lookup<pi_params<float,  8, 1>>);
BENCHMARK(BM_copal_vector_impl_sin_lookup<pi_params<float, 16, 1>>);

BENCHMARK(BM_copal_vector_impl_sin_taylor<pi_params<float,  1, 4>>);
BENCHMARK(BM_copal_vector_impl_sin_taylor<pi_params<float,  1, 2>>);
BENCHMARK(BM_copal_vector_impl_sin_taylor<pi_params<float,  1, 1>>);
BENCHMARK(BM_copal_vector_impl_sin_taylor<pi_params<float,  2, 1>>);
BENCHMARK(BM_copal_vector_impl_sin_taylor<pi_params<float,  4, 1>>);
BENCHMARK(BM_copal_vector_impl_sin_taylor<pi_params<float,  8, 1>>);
BENCHMARK(BM_copal_vector_impl_sin_taylor<pi_params<float, 16, 1>>);

BENCHMARK(BM_copal_vector_stdx_sin_lookup<pi_params<float,  1, 4>>);
BENCHMARK(BM_copal_vector_stdx_sin_lookup<pi_params<float,  1, 2>>);
BENCHMARK(BM_copal_vector_stdx_sin_lookup<pi_params<float,  1, 1>>);
BENCHMARK(BM_copal_vector_stdx_sin_lookup<pi_params<float,  2, 1>>);
BENCHMARK(BM_copal_vector_stdx_sin_lookup<pi_params<float,  4, 1>>);
BENCHMARK(BM_copal_vector_stdx_sin_lookup<pi_params<float,  8, 1>>);
BENCHMARK(BM_copal_vector_stdx_sin_lookup<pi_params<float, 16, 1>>);

BENCHMARK(BM_copal_vector_stdx_sin_taylor<pi_params<float,  1, 4>>);
BENCHMARK(BM_copal_vector_stdx_sin_taylor<pi_params<float,  1, 2>>);
BENCHMARK(BM_copal_vector_stdx_sin_taylor<pi_params<float,  1, 1>>);
BENCHMARK(BM_copal_vector_stdx_sin_taylor<pi_params<float,  2, 1>>);
BENCHMARK(BM_copal_vector_stdx_sin_taylor<pi_params<float,  4, 1>>);
BENCHMARK(BM_copal_vector_stdx_sin_taylor<pi_params<float,  8, 1>>);
BENCHMARK(BM_copal_vector_stdx_sin_taylor<pi_params<float, 16, 1>>);

