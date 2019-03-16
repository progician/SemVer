#include "SemVer/Version.h"
#include <sstream>
#include <iostream>
#include <regex>

namespace SemVer {
  bool operator<(Version const& lhs, Version const& rhs) noexcept {
    if (lhs.major < rhs.major) {
      return true;
    }
    else if (rhs.major < lhs.major) {
      return false;
    }

    if (lhs.minor < rhs.minor) {
      return true;
    }
    else if (rhs.minor < lhs.minor) {
      return false;
    }

    if (lhs.patch < rhs.patch) {
      return true;
    }
    else if (rhs.patch < lhs.patch) {
      return false;
    }

    if (lhs.prerelease.empty() && !rhs.prerelease.empty()) {
      return false;
    }
    else if (!lhs.prerelease.empty() && rhs.prerelease.empty()) {
      return true;
    }

    return lhs.prerelease < rhs.prerelease;
  }

  bool operator==(Version const& lhs, Version const& rhs) noexcept {
    return
      lhs.major == rhs.major &&
      lhs.minor == rhs.minor &&
      lhs.patch == rhs.patch &&
      lhs.prerelease == rhs.prerelease &&
      lhs.build == rhs.build
    ;
  }

  bool operator!=(Version const& lhs, Version const& rhs) noexcept {
    return
      lhs.major != rhs.major ||
      lhs.minor != rhs.minor ||
      lhs.patch != rhs.patch ||
      lhs.prerelease != rhs.prerelease ||
      lhs.build != rhs.build
    ;
  }


  bool operator<=(Version const& lhs, Version const& rhs) noexcept {
    return lhs < rhs || lhs == rhs;
  }

  std::ostream& operator <<(std::ostream& ostr, SemVer::Version const& v) {
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

  std::string Version::to_string() const {
    std::stringstream string_stream;
    string_stream << *this;
    return string_stream.str();
  }

  Version From(std::string const& str) {
    const std::regex parser(R"((\d*)(\.(\d*))?(\.(\d*))?)");
    std::smatch parts;
    if (!std::regex_match(str, parts, parser) || parts.size() < 2) {
      throw std::runtime_error("invalid version range syntax!");
    }

    const int major = std::stoi(parts[1]);
    const int minor = !parts[3].str().empty() ? std::stoi(parts[3]) : 0;
    const int patch = !parts[5].str().empty() ? std::stoi(parts[5]) : 0;
    return {major, minor, patch};
  }
} // SemVer