import sys
from typing import List, Optional


# python __main__.py $(pwd)/2024-07-28-115600.md


class BenchmarkHeading:
    def __init__(self):
        self.instruction_type = "instruction type"
        self.precision = "precision"
        self.test_range = "test range"
        self.implementation_class = "class"
        self.method = "method"
        self.time = "cpu time"
    
    def to_list(self) -> List[str]:
        return [
            self.implementation_class,
            self.method,
            self.instruction_type,
            self.test_range,
            self.precision,
            self.time
        ]

    def to_markdown(self, column_widths: List[int] = [0,0,0,0,0,0]) -> str:
        delim = "|"
        return (
            delim.join([f" {field.ljust(column_widths[i])} " for i, field in enumerate(self.to_list())])
            + "\n"
            + delim.join(["-" * (width + 2) for width in column_widths])
        )


class BenchmarkResult:
    def __init__(self, instruction_type: str, precision: str, test_range_string: str,
                 test_range_value: int, implementation_class: str, method: str, time: int, time_unit: str):
        self.instruction_type = instruction_type
        self.precision = precision
        self.test_range_string = test_range_string
        self.test_range_value = test_range_value
        self.implementation_class = implementation_class
        self.method = method
        self.time = time
        self.time_unit = time_unit

    def to_list(self) -> List[str]:
        return [
            self.implementation_class,
            self.method,
            self.instruction_type,
            self.test_range_string,
            self.precision,
            f"{str(self.time)} {self.time_unit}"
        ]

    def to_markdown(self, column_widths: List[int] = [0,0,0,0,0,0]) -> str:
        delim = "|"
        return delim.join([f" {field.ljust(column_widths[i])} " for i, field in enumerate(self.to_list())])

def parse_benchmark_result(line: str) -> BenchmarkResult:
    parts = line.split()
    print(f"\n{line.split("\n")[0]}")
    i = 0
    for part in parts:
        print(f"  [%i] - %s" % (i, part))
        i += 1

    instruction_type = "simd" if "simd" in parts[0] else "scalar"
    precision = "double" if "double" in parts[1] else "float"

    parts[1] = parts[1].replace(',', '')
    parts[2] = parts[2].replace(',', '')
    if parts[2] == "1":
        test_range_string = f"± {parts[1]} pi"
        test_range_value = float(parts[1])
    else:
        test_range_string = f"+/- {parts[1]}/{parts[2]} pi"
        test_range_value = float(parts[1]) / float(parts[2])

    implementation_class = parts[3].split('<')[0]
    method = parts[3].split('>::')[1].split(">")[0]
    time = int(parts[4])
    time_unit = parts[5]
    return BenchmarkResult(
        instruction_type,
        precision,
        test_range_string,
        test_range_value,
        implementation_class,
        method,
        time,
        time_unit
    )

def measure_column_widths(benchmark_heading_titles: List[str], benchmark_results: List[BenchmarkResult]) -> List[int]:
    assert len(benchmark_heading_titles) > 0, "No benchmark heading titles provided"
    assert len(benchmark_results) > 0, "No benchmark results provided"

    num_heading_columns = len(benchmark_heading_titles)
    column_widths = [len(title) for title in benchmark_heading_titles]

    for benchmark_result in benchmark_results:
        fields = benchmark_result.to_list()
        assert num_heading_columns == len(fields), "BenchmarkHeading and BenchmarkResult must have same number of columns"
        for i, field in enumerate(fields):
            column_widths[i] = len(field) if len(field) > column_widths[i] else column_widths[i]

    return column_widths

def main(file_path: str):
    heading = BenchmarkHeading()
    results: List[BenchmarkResult] = []
    with open(file_path, 'r') as file:
        for line in file:
            if "BM_copal_sin" in line:
                result = parse_benchmark_result(line)
                results.append(result)

    column_widths = measure_column_widths(heading.to_list(), results)

    with open('table-all.md', 'w') as md_file:
        md_file.write(f"{heading.to_markdown(column_widths)}\n")
        for result in results:
            md_file.write(result.to_markdown(column_widths) + '\n')

if __name__ == "__main__":
    main(sys.argv[1])
