#!/usr/bin/env python
from semvercl_testing import exec_cli, find_in_any_lines, test_main
import re, unittest

class AVersionRange(unittest.TestCase):
    abitrary_range = ">=1.0.0"
    semver_command = "match"

    def test_command_is_documented(self):
        _, output_lines, _ = exec_cli(["--help"])
        self.assertTrue( 
            find_in_any_lines(
                "^\\s+{}".format(self.semver_command),
                output_lines
            )
        )
    

    def test_with_version_all_within(self):
        all_within_range = [
            "2.0.0",
            "2.1.0",
            "2.1.1",
            "3.0.0-alpha",
            "3.0.0-beta",
            "3.0.0",
        ]
        command_args = [self.semver_command, self.abitrary_range]
        command_args.extend(all_within_range)
        exit_code, output_lines, _ = exec_cli(command_args)
        self.assertEqual(exit_code, 0)
        self.assertEqual(output_lines, all_within_range) 


    def test_with_some_out_of_range(self):
        some_is_out_of_range = [
            "2.0.0",
            "0.12.0",
            "2.1.0",
            "2.1.1",
            "3.0.0-alpha",
            "1.0.0-beta",
            "3.0.0",
        ]
        within_range = [
            "2.0.0",
            "2.1.0",
            "2.1.1",
            "3.0.0-alpha",
            "3.0.0",
        ]
        command_args = [self.semver_command, self.abitrary_range]
        command_args.extend(some_is_out_of_range)
        _, output_lines, _ = exec_cli(command_args)
        self.assertEqual(output_lines, within_range)
    

    def test_fails_with_invalid_version_strings(self):
        some_is_invalid = [
            "abc",
            "1.0.0",
            "b.a.c",
            "0.1.0-pre.1"
        ]
        command_args = [self.semver_command, self.abitrary_range]
        command_args.extend(some_is_invalid)
        exit_code, _, error_lines = exec_cli(command_args)
        self.assertEqual(exit_code, 1)
        find_in_any_lines(re.compile("^error: .*"), error_lines)

if __name__ == "__main__":
    test_main()
