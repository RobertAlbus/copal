from typing import List

class Table:
    _column_widths: List[int] = []

    def __init__(self, table_data: List[List[str]], has_titles: bool = True):
        self._validate_table_dimensions(table_data)
        self._calculate_column_widths(table_data)
        self._table_data = table_data
        self._has_titles = has_titles

    def to_markdown(self):
        newline = "\n"
        table = self._format_row(self._table_data[0]) + newline
        if self._has_titles:
            table += self._create_header_divider() + newline
        for row in self._table_data[1:]:
            table += self._format_row(row) + newline
        
        return table
    
    def _format_row(self, row: List[str]):
        delim = "|"
        return delim.join([f" {field.ljust(self._column_widths[i])} " for i, field in enumerate(row)])
    
    def _create_header_divider(self):
        delim = "|"
        return delim.join(["-" * (column_width + 2) for column_width in self._column_widths])



    def _validate_table_dimensions(self, table_data: List[List[str]]):
        assert len(table_data) > 0, "Table data must have at least one row"

        first_row = table_data[0]
        first_row_size = len(first_row)
        assert first_row_size > 0, "Table data must have at least one column"

        for i, line in enumerate(table_data):
            assert len(line) == first_row_size, f"Table rows must have equal number of columns. row: {i} - len: {len(line)} - expected: {len(table_data[0])}"

    def _calculate_column_widths(self, table_data: List[List[str]]):
        self._column_widths = [0] * len(table_data[0])
        
        for line in table_data:
            for i, column in enumerate(line):
                size = len(column)
                self._column_widths[i] = size if size > self._column_widths[i] else self._column_widths[i]
        
        # self._column_widths = [x + 2 for x in self._column_widths]

class TableHeader:
    VARIANT              = "variant"
    PRECISION            = "type"
    IMPLEMENTATION_CLASS = "class"
    METHOD               = "method"
    TEST_RANGE           = "test range"
    CPU_TIME             = "cpu time"

    @staticmethod
    def standard() -> List[str]:
        return [
            TableHeader.VARIANT,
            TableHeader.PRECISION,
            TableHeader.IMPLEMENTATION_CLASS,
            TableHeader.METHOD,
            TableHeader.TEST_RANGE,
            TableHeader.CPU_TIME,
        ]