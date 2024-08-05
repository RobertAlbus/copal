from typing import List

from . import BenchmarkResult
from . import TableHeader


def benchmarks_aggregated(benchmarks: List[BenchmarkResult]) -> List[BenchmarkResult]:
    def hash_for_benchmark_family(b: BenchmarkResult) -> str:
        return f"{b.instruction_type}:{b.precision}:{b.implementation_class}:{b.method}"
    
    benchmark_families = set([hash_for_benchmark_family(benchmark) for benchmark in benchmarks])

    aggregates: List[BenchmarkResult] = []

    for benchmark_family in benchmark_families:
        filtered_benchmarks = [benchmark for benchmark in benchmarks if hash_for_benchmark_family(benchmark) == benchmark_family]

        aggregate = BenchmarkResult.copy_from(filtered_benchmarks[0])
        aggregate.test_range_string = "aggregate" 
        aggregate.test_range_value = 0 

        average = sum(benchmark.time for benchmark in filtered_benchmarks) / len(filtered_benchmarks)
        aggregate.time = int(average)

        aggregates.append(aggregate)
    
    return aggregates

def benchmark_sort_by_time(benchmarks: List[BenchmarkResult]) -> None:
    benchmarks.sort(key=lambda benchmark: benchmark.time)

def filter_benchmarks_by_variant_and_precision(benchmarks: List[BenchmarkResult], instruction_types: List[str], precisions: List[str]) -> List[BenchmarkResult]:
    return [
        benchmark for benchmark in benchmarks
            if benchmark.precision in precisions
            and benchmark.instruction_type in instruction_types
    ]

