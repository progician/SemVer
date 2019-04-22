#!/usr/bin/env python
from semvercl_testing import exec_cli, find_in_any_lines, test_main
import re, unittest

class CommandLineOptions(unittest.TestCase):
    def test_when_running_no_option_then_prints_usage(self):
        exit_code, _, error_lines = exec_cli([])
        self.assertEqual(exit_code, 1)
        self.assertEqual(error_lines[0], "error: command required, please use --help")
    
    def test_when_calling_with_help_prints_options(self):
        exit_code, output_lines, _ = exec_cli(["--help"])
        self.assertEqual(exit_code, 0)
        self.assertTrue(find_in_any_lines(re.compile("--help"), output_lines))
        self.assertTrue(find_in_any_lines(re.compile("--version"), output_lines))

    def test_returns_version(self):
        exit_code, output_lines, _ = exec_cli(["--version"])
        self.assertEqual(exit_code, 0)
        self.assertEqual(output_lines[0], "0.1.0")

if __name__ == "__main__":
    test_main()