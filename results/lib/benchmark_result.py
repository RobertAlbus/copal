from __future__ import annotations
from typing import List
import re

from typing import List

class BenchmarkResult:
    def __init__(self, instruction_type: str, precision: str, test_range_string: str,
                 test_range_value: float, implementation_class: str, method: str, time: int, time_unit: str):
        self.instruction_type = instruction_type
        self.precision = precision
        self.test_range_string = test_range_string
        self.test_range_value = test_range_value
        self.implementation_class = implementation_class
        self.method = method
        self.time = time
        self.time_unit = time_unit
    
    @staticmethod
    def from_json(json) -> BenchmarkResult:
        benchmark_result = BenchmarkResult._details_from_name_string(json['name'])

        benchmark_result.time = int(json['cpu_time'])
        benchmark_result.time_unit = json['time_unit']

        return benchmark_result

    @staticmethod
    def from_console_string(line: str) -> BenchmarkResult:
        match = re.search(r'^.*?>>', line)
        assert match is not None, f"Could not parse console line: \n{line}\n"


        benchmark_result = BenchmarkResult._details_from_name_string(match.group())

        parts = line.split()
        benchmark_result.time = int(parts[3])
        benchmark_result.time_unit = parts[4]

        return benchmark_result
    
    @staticmethod
    def _details_from_name_string(name: str) -> BenchmarkResult:
        parts = name.split()
    
        instruction_type = next((instruction_type for instruction_type in ["scalar", "simd"] if instruction_type in parts[0]), None)
        assert instruction_type is not None, "BenchmarkResult instruction_type must be one of [scalar, simd]"
        
        precision = next((precision for precision in ["float", "double"] if precision in parts[0]), None)
        assert precision is not None, "BenchmarkResult precision must be one of [float, double]"

        multiplier = parts[1].replace(',', '')
        test_range_string = f"± {multiplier} pi"
        test_range_value = float(multiplier) * 3.14159
        implementation_class = parts[2].split('<')[0]
        method = parts[2].split('>::')[1].replace('>>', '')

        return BenchmarkResult(
            instruction_type,
            precision,
            test_range_string,
            test_range_value,
            implementation_class,
            method,
            0, ""
        )

    def to_list(self) -> List[str]:
        return [
            self.instruction_type,
            self.precision,
            self.implementation_class,
            self.method,
            self.test_range_string,
            f"{str(self.time)} {self.time_unit}"
        ]
