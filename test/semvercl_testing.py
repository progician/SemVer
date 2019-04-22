import locale
import logging
import subprocess
import sys
import unittest
import re


def exec_any_cli(program, args):
    command = [program]
    command.extend(args)

    logging.getLogger().info("+%s", str(command))
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
