#!/usr/bin/env python
from semvercl_testing import exec_any_cli, find_in_any_lines
import re
import pytest

def test_when_running_no_option_then_prints_usage(semver_exec):
    exit_code, _, error_lines = exec_any_cli(semver_exec, [])
    assert exit_code == 1
    assert error_lines[0] == "error: command required, please use --help"

def test_when_calling_with_help_prints_options(semver_exec):
    exit_code, output_lines, _ = exec_any_cli(semver_exec, ["--help"])
    assert exit_code == 0
    assert find_in_any_lines(re.compile("--help"), output_lines)
    assert find_in_any_lines(re.compile("--version"), output_lines)

def test_returns_version(semver_exec):
    exit_code, output_lines, _ = exec_any_cli(semver_exec, ["--version"])
    assert exit_code == 0
    assert output_lines[0] == "0.1.0"