#!/usr/bin/env python
import locale, subprocess, sys, unittest
import re

def _find_and_remove_args(argv, setting):
    prefix = "--{}=".format(setting)
    for i in range(1, len(argv)):
        if argv[i].startswith(prefix):
            value = argv[i][len(prefix):]
            del argv[i]
            return value
    return None

def _strip_test_suite_args(argv, setting_list):
    settings = {}
    for setting in setting_list:
        value = _find_and_remove_args(argv, setting)
        if value is not None:
            settings[setting] = value
    return settings

settings = {}

def _exec_cli(args):
    command = [settings["semver-cli-exec"]]
    command.extend(args)
    completed = subprocess.run(
        command,
        encoding=locale.getpreferredencoding(),
        stderr=subprocess.PIPE,
        stdout=subprocess.PIPE
    )
    output_lines = str(completed.stdout).split('\n')
    error_lines = str(completed.stderr).split('\n')
    return completed.returncode, output_lines, error_lines

def _find_in_any_lines(pattern, lines):
    for l in lines:
        if re.search(pattern, l) is not None:
            return True
    return False


class CommandLineOptions(unittest.TestCase):
    def test_when_running_no_option_then_prints_usage(self):
        exit_code, _, error_lines = _exec_cli([])
        self.assertEqual(exit_code, 1)
        self.assertEqual(error_lines[0], "error: command required, please use --help")
    
    def test_when_calling_with_help_prints_options(self):
        exit_code, output_lines, _ = _exec_cli(["--help"])
        self.assertEqual(exit_code, 0)
        self.assertTrue(_find_in_any_lines(re.compile("--help"), output_lines))
        self.assertTrue(_find_in_any_lines(re.compile("--version"), output_lines))

    def test_returns_version(self):
        exit_code, output_lines, _ = _exec_cli(["--version"])
        self.assertEqual(exit_code, 0)
        self.assertEqual(output_lines[0], "0.1.0")

if __name__ == "__main__":
    settings = _strip_test_suite_args(sys.argv, ["semver-cli-exec"])
    unittest.main()