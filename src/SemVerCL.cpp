#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
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


using ArgumentRange = std::pair<const char**, const char**>;
auto HasOption(ArgumentRange args, const char* option) -> bool{
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


void OrderVersions(ArgumentRange args) {
  std::multiset<SemVer::Version> parsed_versions;
  std::transform(
      args.first, args.second,
      std::inserter(parsed_versions, std::end(parsed_versions)),
      SemVer::From
  );
  std::copy(
      std::begin(parsed_versions), std::end(parsed_versions),
      std::ostream_iterator<SemVer::Version>(std::cout, "\n")
  );
}


void FilterMatches(ArgumentRange args) {
  if (args.second - args.first < 2)
    throw std::invalid_argument("missing version range specification");

  auto const range = SemVer::RangeFrom(args.first[0]);
  std::copy_if(
      args.first + 1, args.second,
      std::ostream_iterator<const char*>(std::cout, "\n"),
      [&range](const char* argument) {
        auto const version = SemVer::From(argument);
        return SemVer::Match(version, range);
      }
  );
}


using CommandFunction = std::function<void(ArgumentRange)>;
const auto Commands = std::map<std::string, CommandFunction>{
  {"order", OrderVersions},
  {"match", FilterMatches},
};


auto main(int argc, const char* argv[]) -> int {
  ArgumentRange sys_args(argv, argv + argc);
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
    if (argc < 2)
      throw std::runtime_error("command required, please use --help");

    auto command_function = Commands.find(std::string(argv[1]));
    if (command_function == std::end(Commands))
      throw std::runtime_error("unknown command");

    command_function->second(ArgumentRange{argv + 2, argv + argc});
  }
  catch (std::exception const& e) {
    std::cerr << "error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
