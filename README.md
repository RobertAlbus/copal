# Copal
This repositor is permissively licensed. See [License](./LICENSE)

* [About This Repository](#about-this-repository)
* [Results Summary](#results-summary)
* [Benchmarking](#benchmarking)
* [Quick Start](#quick-start)
* [Repository Layout](#repository-layout)

## About This Repository
This repository hosts a dockerized C++ 20 project exploring the performance of scalar and SIMD implementations of sin(x). Benchmarks include both standard library and custom implementations for the sine function and underlying math operations.

Dependencies:
* [Google Test](https://github.com/google/googletest)
* [Google Benchmark](https://github.com/google/benchmark)

## Results Summary
Detailed benchmarking output is located in [/copal/results](/copal/results/2024-07-10-optimized.md).

I was only able to beat the speed of `std::experimental::simd` overridden 



BM_copal_sin_simd<bm_simd_sin_params<float,  1, 4, copal::VectorImpl<float>::sin_taylor>>          1395 ns         1391 ns      525688
BM_copal_sin_simd<bm_simd_sin_params<float,  1, 2, copal::VectorImpl<float>::sin_taylor>>          1346 ns         1342 ns      525953
BM_copal_sin_simd<bm_simd_sin_params<float,  1, 1, copal::VectorImpl<float>::sin_taylor>>          1358 ns         1354 ns      520884
BM_copal_sin_simd<bm_simd_sin_params<float,  2, 1, copal::VectorImpl<float>::sin_taylor>>          1335 ns         1332 ns      527450
BM_copal_sin_simd<bm_simd_sin_params<float,  4, 1, copal::VectorImpl<float>::sin_taylor>>          3524 ns         3514 ns      201830
BM_copal_sin_simd<bm_simd_sin_params<float,  8, 1, copal::VectorImpl<float>::sin_taylor>>          3365 ns         3356 ns      208632
BM_copal_sin_simd<bm_simd_sin_params<float, 16, 1, copal::VectorImpl<float>::sin_taylor>>          3344 ns 

BM_copal_sin_simd<bm_simd_sin_params<float,  1, 4, copal::VectorStdx<float>::sin_taylor>>          1316 ns         1313 ns      518098
BM_copal_sin_simd<bm_simd_sin_params<float,  1, 2, copal::VectorStdx<float>::sin_taylor>>          1314 ns         1310 ns      532938
BM_copal_sin_simd<bm_simd_sin_params<float,  1, 1, copal::VectorStdx<float>::sin_taylor>>          1347 ns         1344 ns      528864
BM_copal_sin_simd<bm_simd_sin_params<float,  2, 1, copal::VectorStdx<float>::sin_taylor>>          1374 ns         1370 ns      527974
BM_copal_sin_simd<bm_simd_sin_params<float,  4, 1, copal::VectorStdx<float>::sin_taylor>>         10547 ns        10512 ns       67268
BM_copal_sin_simd<bm_simd_sin_params<float,  8, 1, copal::VectorStdx<float>::sin_taylor>>         10467 ns        10434 ns       67116
BM_copal_sin_simd<bm_simd_sin_params<float, 16, 1, copal::VectorStdx<float>::sin_taylor>>         10450 ns        10421 ns       66203

BM_copal_sin_simd<bm_simd_sin_params<float,  1, 4, copal::VectorStdx<float>::sin_stdlib>>           811 ns          809 ns      866232
BM_copal_sin_simd<bm_simd_sin_params<float,  1, 2, copal::VectorStdx<float>::sin_stdlib>>          1376 ns         1372 ns      521572
BM_copal_sin_simd<bm_simd_sin_params<float,  1, 1, copal::VectorStdx<float>::sin_stdlib>>          1361 ns         1357 ns      522169
BM_copal_sin_simd<bm_simd_sin_params<float,  2, 1, copal::VectorStdx<float>::sin_stdlib>>          1984 ns         1978 ns      356163
BM_copal_sin_simd<bm_simd_sin_params<float,  4, 1, copal::VectorStdx<float>::sin_stdlib>>          1989 ns         1984 ns      356721
BM_copal_sin_simd<bm_simd_sin_params<float,  8, 1, copal::VectorStdx<float>::sin_stdlib>>          2004 ns         1998 ns      354274
BM_copal_sin_simd<bm_simd_sin_params<float, 16, 1, copal::VectorStdx<float>::sin_stdlib>>          1965 ns         1960 ns      356911




## Benchmarking
Benchmarks for sin(x) include a lookup table, Taylor series approximation, and `std::sin`, for both scalar and SIMD implementations. Tests span seven different input ranges, from ±1/4 pi to ±16 pi, totaling over 84 distinct benchmarks. The findings indicate specific scenarios where custom implementations using SIMD outperform traditional methods, particularly in data parallel approaches for specific input ranges.

Benchmarks can be found in the directory [copal/results](copal/results)

## Quick Start
1. Ensure Docker is installed and configured.
2. Clone the repository and navigate to the project directory.
3. [Build the provided Docker images](docker/README.md) to set up all necessary dependencies.
4. [Compile the code](copal/BUILD.md)
5. Execute the benchmarks or tests with `./copal/build/copal-benchmark` or `./copal/build/copal-test` respectively.

## Repository Layout
- **copal/**: C++ code
    - **/include**: Templated implementations
    - **/benchmark**: Performance benchmarks
    - **/test**: Correctness tests
- **docker**: Dockerfiles and scripts for building dependencies, setting up development environment, and compiling the code
