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

def exec_cli(args):
    command = [settings["semver-cli-exec"]]
    command.extend(args)
    if settings.get("semver-verbosity", "quiet") == "verbose":
        print("+" + str(command))
    completed = subprocess.run(
        command,
        encoding=locale.getpreferredencoding(),
        stderr=subprocess.PIPE,
        stdout=subprocess.PIPE
    )
    output_lines = str(completed.stdout).split('\n')
    if output_lines[-1] == "":
        output_lines.pop()
    error_lines = str(completed.stderr).split('\n')
    return completed.returncode, output_lines, error_lines

def exec_any_cli(program, args):
    command = [program]
    command.extend(args)
    if settings.get("semver-verbosity", "quiet") == "verbose":
        print("+" + str(command))
    completed = subprocess.run(
        command,
        encoding=locale.getpreferredencoding(),
        stderr=subprocess.PIPE,
        stdout=subprocess.PIPE
    )
    output_lines = str(completed.stdout).split('\n')
    if output_lines[-1] == "":
        output_lines.pop()
    error_lines = str(completed.stderr).split('\n')
    return completed.returncode, output_lines, error_lines

def find_in_any_lines(pattern, lines):
    for l in lines:
        if re.search(pattern, l) is not None:
            return True
    return False

def test_main():
    global settings
    settings = _strip_test_suite_args(sys.argv, ["semver-cli-exec", "semver-verbosity"])
    unittest.main()