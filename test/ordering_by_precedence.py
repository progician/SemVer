#!/usr/bin/env python
from semvercl_testing import exec_cli, find_in_any_lines, test_main
import re, unittest

class OrderingByPrecedence(unittest.TestCase):
    def test_command_is_documented(self):
        _, output_lines, _ = exec_cli(["--help"])
        self.assertTrue(find_in_any_lines("^\\s+order", output_lines))

if __name__ == "__main__":
    test_main()