import sys
from typing import List

from lib import Table, BenchmarkResult


# python __main__.py $(pwd)/2024-07-10-optimized.md


standard_header = [
    "instruction type",
    "precision",
    "test range",
    "class",
    "method",
    "cpu time",
]

def main(file_path: str):
    results: List[BenchmarkResult] = []
    with open(file_path, 'r') as file:
        for line in file:
            if "BM_copal_sin" in line:
                results.append(
                    BenchmarkResult.from_console_string(line)
                )

    table = Table(
        table_data=[standard_header] + [result.to_list() for result in results],
        has_titles=True
    )

    with open('table-all.md', 'w') as md_file:
        md_file.write(table.to_markdown())

if __name__ == "__main__":
    main(sys.argv[1])
