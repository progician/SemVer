#include "SemVer/Version.h"
#include <sstream>
#include <iostream>
#include <regex>

namespace SemVer {
  auto operator<(Version const& lhs, Version const& rhs) noexcept -> bool {
    if (lhs.major < rhs.major) {
      return true;
    }
    if (rhs.major < lhs.major) {
      return false;
    }

    if (lhs.minor < rhs.minor) {
      return true;
    }
    if (rhs.minor < lhs.minor) {
      return false;
    }

    if (lhs.patch < rhs.patch) {
      return true;
    }
    if (rhs.patch < lhs.patch) {
      return false;
    }

    if (lhs.prerelease.empty() && !rhs.prerelease.empty()) {
      return false;
    }
    if (!lhs.prerelease.empty() && rhs.prerelease.empty()) {
      return true;
    }

    return lhs.prerelease < rhs.prerelease;
  }

  auto operator==(Version const& lhs, Version const& rhs) noexcept -> bool {
    return
      lhs.major == rhs.major &&
      lhs.minor == rhs.minor &&
      lhs.patch == rhs.patch &&
      lhs.prerelease == rhs.prerelease &&
      lhs.build == rhs.build
    ;
  }

  auto operator!=(Version const& lhs, Version const& rhs) noexcept -> bool {
    return
      lhs.major != rhs.major ||
      lhs.minor != rhs.minor ||
      lhs.patch != rhs.patch ||
      lhs.prerelease != rhs.prerelease ||
      lhs.build != rhs.build
    ;
  }


  auto operator<=(Version const& lhs, Version const& rhs) noexcept -> bool {
    return lhs < rhs || lhs == rhs;
  }

  auto operator <<(std::ostream& ostr, SemVer::Version const& v) -> std::ostream& {
    ostr << v.major << ".";
    ostr << v.minor << ".";
    ostr << v.patch;
    if (!v.prerelease.empty()) {
      ostr << "-" << v.prerelease;
    }
    if (!v.build.empty()) {
      ostr << "+" << v.build;
    }
    return ostr;
  }

  auto to_string(Version const& v) -> std::string {
    std::stringstream stream;
    stream << v;
    return stream.str();
  }

  auto From(std::string const& str) -> Version {
    const std::regex parser(R"((\d*)(\.(\d*))?(\.(\d*))?(-([a-zA-Z0-9\-][\dA-Za-z\-\.]*))?(\+([a-zA-Z0-9][\dA-Za-z\-\.]*))?)");
    std::smatch parts;
    if (!std::regex_match(str, parts, parser) || parts.size() < 2) {
      throw std::invalid_argument("invalid version range syntax!");
    }

    const int major = std::stoi(parts[1]);
    const int minor = !parts[3].str().empty() ? std::stoi(parts[3]) : 0;
    const int patch = !parts[5].str().empty() ? std::stoi(parts[5]) : 0;
    return {major, minor, patch, parts[7].str(), parts[9].str()};
  }
} // SemVer