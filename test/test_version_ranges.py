#!/usr/bin/env python
from semvercl_testing import exec_any_cli, find_in_any_lines
import pytest
import re

abitrary_range = ">=1.0.0"
semver_command = "match"


def test_command_is_documented(semver_exec):
    _, output_lines, _ = exec_any_cli(semver_exec, ["--help"])
    assert find_in_any_lines(
        "^\\s+{}".format(semver_command),
        output_lines
    )


def test_with_version_all_within(semver_exec):
    all_within_range = [
        "2.0.0",
        "2.1.0",
        "2.1.1",
        "3.0.0-alpha",
        "3.0.0-beta",
        "3.0.0",
    ]
    command_args = [semver_command, abitrary_range]
    command_args.extend(all_within_range)
    exit_code, output_lines, _ = exec_any_cli(semver_exec, command_args)
    assert exit_code == 0
    assert output_lines == all_within_range


def test_with_some_out_of_range(semver_exec):
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
    command_args = [semver_command, abitrary_range]
    command_args.extend(some_is_out_of_range)
    _, output_lines, _ = exec_any_cli(semver_exec, command_args)
    assert output_lines == within_range


def test_fails_with_invalid_version_strings(semver_exec):
    some_is_invalid = [
        "abc",
        "1.0.0",
        "b.a.c",
        "0.1.0-pre.1"
    ]
    command_args = [semver_command, abitrary_range]
    command_args.extend(some_is_invalid)
    exit_code, _, _ = exec_any_cli(semver_exec, command_args)
    assert exit_code == 1
