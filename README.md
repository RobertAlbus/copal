# Copal
This repositor is permissively licensed. See [License](./LICENSE)

## About This Repository
This repository hosts a dockerized C++ 20 project exploring the performance of scalar and SIMD implementations of sin(x). Benchmarks include both standard library and custom implementations for the sine function and underlying math operations.

Dependencies:
* [Google Test](https://github.com/google/googletest)
* [Google Benchmark](https://github.com/google/benchmark)

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
