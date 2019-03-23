#include <algorithm>
#include <iostream>
#include <string>
#include <utility>

#include "SemVer/VersionNumbers.h"

namespace {
  using ArgumentRange = std::pair<const char**, const char**>;
  bool HasOption(ArgumentRange args, const char* option) {
    return std::find(args.first, args.second, std::string(option)) != args.second;
  }
}

int main(int argc, const char* argv[]) {
  ArgumentRange sys_args(argv, argv + argc);
  if (argc < 2) {
    std::cerr << "error: command required, please use --help" << std::endl;
    return 1;
  }

  if (HasOption(sys_args, "--help")) {
    std::cout << "you called --help" << std::endl;
    return 0;
  }

  if (HasOption(sys_args, "--version")) {
    std::cout << SemVer::VersionMajor
              << "." << SemVer::VersionMinor
              << "." << SemVer::VersionPatch
              << std::endl;
    return 0;
  }

  std::cerr << "error: unknown command" << std::endl;
  return 1;
}