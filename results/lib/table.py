from typing import List, Optional

class Table:

    def __init__(self, table_data: List[List[str]], has_titles: bool = True, alignments: Optional[str] = None):
        self._validate_table(table_data, alignments)
        self._column_widths = self._calculate_column_widths(table_data)
        self._table_data = table_data
        self._has_titles = has_titles
        self._alignments = alignments or self._default_alignments()

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
        fields: List[str] = []
        for i, field in enumerate(row):
            if self._alignments[i] == 'L':
                fields.append(field.ljust(self._column_widths[i]))
            elif self._alignments[i] == 'R':
                fields.append(field.rjust(self._column_widths[i]))
            elif self._alignments[i] == 'C':
                fields.append(field.center(self._column_widths[i]))
            else:
                assert False, "Should never assert"

        return delim.join([f" {field} " for field in fields])
        
    def _create_header_divider(self):
        sections = ["-" * (column_width + 2) for column_width in self._column_widths]
        for i, _ in enumerate(sections):
            if self._alignments[i] == 'L':
                sections[i] = ":" + sections[i][1:]
            elif self._alignments[i] == 'R':
                sections[i] = sections[i][:-1] + ":"
            elif self._alignments[i] == 'C':
                sections[i] = ":" + sections[i][1:]
                sections[i] = sections[i][:-1] + ":"
            else:
                assert False, "Should never assert"

        delim = "|"
        return delim.join(sections)
    
    def _default_alignments(self) -> str:
        return ("L") * len(self._column_widths)

    def _validate_table(self, table_data: List[List[str]], alignments: Optional[str]):
        assert len(table_data) > 0, "Table data must have at least one row"

        first_row = table_data[0]
        first_row_size = len(first_row)
        assert first_row_size > 0, "Table data must have at least one column"

        for i, line in enumerate(table_data):
            assert len(line) == first_row_size, f"Table rows must have equal number of columns. row: {i} - len: {len(line)} - expected: {len(table_data[0])}"
        
        if alignments is not None:
            assert first_row_size == len(alignments), "Table alignment must be provided for all columns, or else None"
            for alignment in alignments:
                assert alignment in ["L", "R", "C"], "Table column alignment must be one of L R C"

    def _calculate_column_widths(self, table_data: List[List[str]]) -> List[int]:
        column_widths = [0] * len(table_data[0])
        
        for line in table_data:
            for i, column in enumerate(line):
                size = len(column)
                column_widths[i] = size if size > column_widths[i] else column_widths[i]
        
        return column_widths

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