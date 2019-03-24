from semvercl_testing import exec_cli, find_in_any_lines, test_main
import re, unittest

class OrderingByPrecedence(unittest.TestCase):
    def test_command_is_documented(self):
        _, output_lines, _ = exec_cli(["--help"])
        self.assertTrue(find_in_any_lines("^\\s+order", output_lines))
    
    def test_when_version_range(self):
        all_within_range = [
            "2.0.0",
            "2.1.0",
            "2.1.1",
            "3.0.0-alpha",
            "3.0.0-beta",
            "3.0.0",
        ]
        command_args = ["range", ">=1.0.0"]
        command_args.extend(all_within_range)
        exit_code, _, _ = exec_cli(command_args)
        self.assertEqual(exit_code, 0)

if __name__ == "__main__":
    test_main()
