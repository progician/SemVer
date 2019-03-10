#include "SemVer/Version.h"
#include <sstream>

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
} // SemVer