import json
import sys

from typing import List

from lib import Table, TableHeader, BenchmarkResult, benchmarks_aggregated, benchmark_sort_by_time, filter_benchmarks_by_variant_and_precision


# python __main__.py $(pwd)/2024-08-01-201100.json




def main(file_path: str):
    benchmarks: List[BenchmarkResult] = []
    with open(file_path, 'r') as file:
        if ".json" in file_path:
            data = json.load(file)
            benchmarks_json = data['benchmarks']
            for benchmark in benchmarks_json:
                benchmarks.append(
                    BenchmarkResult.from_json(benchmark)
                )
        else:
            for line in file:
                if "BM_copal_sin" in line:
                    benchmarks.append(
                        BenchmarkResult.from_console_string(line)
                    )
    
    with open('table-all.md', 'w') as md_file:
        md_file.write(
            Table(
                table_data=[TableHeader.standard()] + [benchmark.to_list() for benchmark in benchmarks],
                has_titles=True,
                alignments="LLLLRR"
            ).to_markdown()
        )

    aggregated_benchmarks = benchmarks_aggregated(benchmarks)
    benchmark_sort_by_time(aggregated_benchmarks)

    column_test_range = TableHeader.standard().index(TableHeader.TEST_RANGE)

    ## median speed: [scalar, SIMD] [float, double]
    median_speed_table_data = [TableHeader.standard()] + [benchmark.to_list() for benchmark in aggregated_benchmarks]
    
    for line in median_speed_table_data:
        line.pop(column_test_range)

    ## median speed: [scalar, SIMD] float
    aggregated_benchmarks_float = filter_benchmarks_by_variant_and_precision(aggregated_benchmarks, ["scalar", "simd"], ["float"])
    aggregated_benchmarks_float_table_data = [TableHeader.standard()] + [benchmark.to_list() for benchmark in aggregated_benchmarks_float]
    for line in aggregated_benchmarks_float_table_data:
        line.pop(column_test_range)

    ## median speed: [scalar, SIMD] double
    aggregated_benchmarks_double = filter_benchmarks_by_variant_and_precision(aggregated_benchmarks, ["scalar", "simd"], ["double"])
    aggregated_benchmarks_double_table_data = [TableHeader.standard()] + [benchmark.to_list() for benchmark in aggregated_benchmarks_double]
    for line in aggregated_benchmarks_double_table_data:
        line.pop(column_test_range)
    
    ## median speed: scalar [float, double]
    aggregated_benchmarks_scalar = filter_benchmarks_by_variant_and_precision(aggregated_benchmarks, ["scalar"], ["float", "double"])
    aggregated_benchmarks_scalar_table_data = [TableHeader.standard()] + [benchmark.to_list() for benchmark in aggregated_benchmarks_scalar]
    for line in aggregated_benchmarks_scalar_table_data:
        line.pop(column_test_range)

    ## median speed: scalar float
    aggregated_benchmarks_scalar_float = filter_benchmarks_by_variant_and_precision(aggregated_benchmarks, ["scalar"], ["float"])
    aggregated_benchmarks_scalar_float_table_data = [TableHeader.standard()] + [benchmark.to_list() for benchmark in aggregated_benchmarks_scalar_float]
    for line in aggregated_benchmarks_scalar_float_table_data:
        line.pop(column_test_range)

    ## median speed: scalar double
    aggregated_benchmarks_scalar_double = filter_benchmarks_by_variant_and_precision(aggregated_benchmarks, ["scalar"], ["double"])
    aggregated_benchmarks_scalar_double_table_data = [TableHeader.standard()] + [benchmark.to_list() for benchmark in aggregated_benchmarks_scalar_double]
    for line in aggregated_benchmarks_scalar_double_table_data:
        line.pop(column_test_range)

    ## median speed: SIMD [float, double]
    aggregated_benchmarks_simd = filter_benchmarks_by_variant_and_precision(aggregated_benchmarks, ["simd"], ["float", "double"])
    aggregated_benchmarks_simd_table_data = [TableHeader.standard()] + [benchmark.to_list() for benchmark in aggregated_benchmarks_simd]
    for line in aggregated_benchmarks_simd_table_data:
        line.pop(column_test_range)

    ## median speed: SIMD float
    aggregated_benchmarks_simd_float = filter_benchmarks_by_variant_and_precision(aggregated_benchmarks, ["simd"], ["float"])
    aggregated_benchmarks_simd_float_table_data = [TableHeader.standard()] + [benchmark.to_list() for benchmark in aggregated_benchmarks_simd_float]
    for line in aggregated_benchmarks_simd_float_table_data:
        line.pop(column_test_range)

    ## median speed: SIMD double
    aggregated_benchmarks_simd_double = filter_benchmarks_by_variant_and_precision(aggregated_benchmarks, ["simd"], ["double"])
    aggregated_benchmarks_simd_double_table_data = [TableHeader.standard()] + [benchmark.to_list() for benchmark in aggregated_benchmarks_simd_double]
    for line in aggregated_benchmarks_simd_double_table_data:
        line.pop(column_test_range)

    
    with open('table-ranked-median-speed.md', 'w') as md_file:
        md_file.write("# Ranked by Median Speed")
        md_file.write("\n\n## [scalar, SIMD] [float, double] Benchmarks\n")
        md_file.write(
            Table(
                table_data=median_speed_table_data,
                has_titles=True,
                alignments="LLLLR"
            ).to_markdown()
        )

        md_file.write("\n\n## [scalar, SIMD] float Benchmarks\n")
        md_file.write(
            Table(
                table_data=aggregated_benchmarks_float_table_data,
                has_titles=True,
                alignments="LLLLR"
            ).to_markdown()
        )

        md_file.write("\n\n## [scalar, SIMD] double Benchmarks\n")
        md_file.write(
            Table(
                table_data=aggregated_benchmarks_double_table_data,
                has_titles=True,
                alignments="LLLLR"
            ).to_markdown()
        )

        md_file.write("\n\n## scalar [float, double] Benchmarks\n")
        md_file.write(
            Table(
                table_data=aggregated_benchmarks_scalar_table_data,
                has_titles=True,
                alignments="LLLLR"
            ).to_markdown()
        )

        md_file.write("\n\n## scalar float Benchmarks\n")
        md_file.write(
            Table(
                table_data=aggregated_benchmarks_scalar_float_table_data,
                has_titles=True,
                alignments="LLLLR"
            ).to_markdown()
        )

        md_file.write("\n\n## scalar double Benchmarks\n")
        md_file.write(
            Table(
                table_data=aggregated_benchmarks_scalar_double_table_data,
                has_titles=True,
                alignments="LLLLR"
            ).to_markdown()
        )

        md_file.write("\n\n## SIMD [float, double] Benchmarks\n")
        md_file.write(
            Table(
                table_data=aggregated_benchmarks_simd_table_data,
                has_titles=True,
                alignments="LLLLR"
            ).to_markdown()
        )

        md_file.write("\n\n## SIMD float Benchmarks\n")
        md_file.write(
            Table(
                table_data=aggregated_benchmarks_simd_float_table_data,
                has_titles=True,
                alignments="LLLLR"
            ).to_markdown()
        )

        md_file.write("\n\n## SIMD double Benchmarks\n")
        md_file.write(
            Table(
                table_data=aggregated_benchmarks_simd_double_table_data,
                has_titles=True,
                alignments="LLLLR"
            ).to_markdown()
        )

    # results_ranked_speed_by_input_range = {}
    #     # dict:
    #         # keys: input range string
    #         # values: List[BenchmarkResult]


if __name__ == "__main__":
    main(sys.argv[1])
