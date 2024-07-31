import sys
from typing import List

from lib import Table


# python __main__.py $(pwd)/2024-07-10-optimized.md


standard_header = [
    "instruction type",
    "precision",
    "test range",
    "class",
    "method",
    "cpu time",
]

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

    instruction_type = "simd" if "simd" in parts[0] else "scalar"
    precision = "double" if "double" in parts[1] else "float"

    parts[1] = parts[1].replace(',', '')
    parts[2] = parts[2].replace(',', '')
    if parts[2] == "1":
        test_range_string = f"± {parts[1]} pi"
        test_range_value = float(parts[1])
    else:
        test_range_string = f"± {parts[1]}/{parts[2]} pi"
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

def main(file_path: str):
    results: List[BenchmarkResult] = []
    with open(file_path, 'r') as file:
        for line in file:
            if "BM_copal_sin" in line:
                result = parse_benchmark_result(line)
                results.append(result)

    table = Table(
        table_data=[standard_header] + [result.to_list() for result in results],
        has_titles=True
    )

    with open('table-all.md', 'w') as md_file:
        md_file.write(table.to_markdown())

if __name__ == "__main__":
    main(sys.argv[1])
