#!/usr/bin/env python
from semvercl_testing import exec_cli, find_in_any_lines, test_main
import random, re, unittest

class OrderingByPrecedence(unittest.TestCase):
    def test_command_is_documented(self):
        _, output_lines, _ = exec_cli(["--help"])
        self.assertTrue(find_in_any_lines("^\\s+order", output_lines))
    
    def test_order_arguments(self):
        ordered_versions = [
            "1.0.0",
            "2.0.0",
            "2.1.0",
            "2.1.1",
            "3.0.0-alpha",
            "3.0.0-beta",
            "3.0.0",
        ]
        randomised_versions = ordered_versions.copy()
        random.shuffle(randomised_versions)
        command_args = ["order"]
        command_args.extend(randomised_versions)
        _, output_lines, _ = exec_cli(command_args)
        self.assertEqual(output_lines, ordered_versions)

if __name__ == "__main__":
    test_main()