#!/usr/bin/env python
import locale, subprocess, sys, unittest

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

    completed = subprocess.run(
        [settings["semver-cli-exec"]],
        encoding=locale.getpreferredencoding(),
        stderr=subprocess.PIPE,
        stdout=subprocess.PIPE
    )
    output_lines = str(completed.stdout).split('\n')
    error_lines = str(completed.stderr).split('\n')
    return completed.returncode, output_lines, error_lines

class CommandLineOptions(unittest.TestCase):
    def test_when_running_no_option_then_prints_usage(self):
        exit_code, _, error_lines = _exec_cli(["something"])
        self.assertEqual(exit_code, 1)
        self.assertEqual(error_lines[0], "error: command required, please use --help")

if __name__ == "__main__":
    settings = _strip_test_suite_args(sys.argv, ["semver-cli-exec"])
    unittest.main()