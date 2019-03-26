#include <algorithm>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>

#include "SemVer/Version.h"
#include "SemVer/VersionNumbers.h"
#include "SemVer/VersionRange.h"

static const SemVer::Version SemVerProjectVersion{
  SemVer::VersionMajor, SemVer::VersionMinor, SemVer::VersionPatch
};

namespace {
  using ArgumentRange = std::pair<const char**, const char**>;
  bool HasOption(ArgumentRange args, const char* option) {
    return std::find(args.first, args.second, std::string(option)) != args.second;
  }

  void PrintUsage(std::string const& executable_name, std::ostream& target) {
    target << "semver-cl " << SemVerProjectVersion << std::endl
           << "  Simple command line client front end for semantic version processing." << std::endl
           << "usage: " << executable_name << " [OPTS] COMMAND [ARGS]" << std::endl
           << "options:" << std::endl
           << "    -h, --help" << std::endl
           << "        Print this help screen." << std::endl
           << "    -v, --version" << std::endl
           << "        Print the version number of SemVer." << std::endl
           << "commands:" << std::endl
           << "    order [ARGS]" << std::endl
           << "        sort the rest of the arguments in precedence order or"
              " the standard input" << std::endl
           << "    match VERSION_RANGE [ARGS]" << std::endl
           << "        use the VERSION_RANGE to match against the rest of the arguments"
              " in precedence order or the standard input" << std::endl
           << "        All the matching version will be listed in the output."
           << std::endl
    ;
  }
}


int main(int argc, const char* argv[]) {
  ArgumentRange sys_args(argv, argv + argc);
  if (argc < 2) {
    std::cerr << "error: command required, please use --help" << std::endl;
    return 1;
  }

  if (HasOption(sys_args, "--help")) {
    PrintUsage(argv[0], std::cout);
    return 0;
  }

  if (HasOption(sys_args, "--version")) {
    std::cout << SemVer::VersionMajor
              << "." << SemVer::VersionMinor
              << "." << SemVer::VersionPatch
              << std::endl;
    return 0;
  }

  try {
    if (std::string(argv[1]) == "order") {
      std::multiset<SemVer::Version> parsed_versions;
      for (int argument = 2; argument < argc; argument++) {
        parsed_versions.insert(SemVer::From(argv[argument]));
      }
      for (auto const& v : parsed_versions) {
        std::cout << v << std::endl;
      }
      return 0;
    }
    else if (std::string(argv[1]) == "match") {
      if (argc < 3) {
        std::cerr << "error: missing version range specification" << std::endl;
        return 1;
      }

      auto const range = SemVer::RangeFrom(argv[2]);
      std::copy_if(
          argv + 3, argv + argc,
          std::ostream_iterator<const char*>(std::cout, "\n"),
          [&range](const char* argument) {
            auto const version = SemVer::From(argument);
            return SemVer::Match(version, range);
          }
      );
      return 0;
    }
  }
  catch (std::logic_error e) {
    std::cerr << "error: " << e.what() << std::endl;
    return 1;
  }

  std::cerr << "error: unknown command" << std::endl;
  return 1;
}