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
    

    def test_any_bad_parsing_causes_error_message(self):
        badly_written_versions = [
            "1.0.0",
            "2asf.0.0",
            "2.1.0",
            "2.1.1",
            "3.0.0-alpha",
            "3.0.0-beta",
            "***---...3.0.0",
        ]
        command_args = ["order"]
        command_args.extend(badly_written_versions)
        exit_code, _, error_lines = exec_cli(command_args)
        self.assertNotEqual(exit_code, 0)
        self.assertTrue(find_in_any_lines(re.compile("^error:.*"), error_lines))


if __name__ == "__main__":
    test_main()