from typing import List

from typing import List

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