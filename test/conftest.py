import pytest


def pytest_addoption(parser):
    parser.addoption(
        "--semver-cli-exec", action="store", default="../build/semver-cl",
        help="SemVer command line executable"
    )


@pytest.fixture
def semver_exec(request):
    return request.config.getoption("--semver-cli-exec")
