#pragma once

#include <initializer_list>

namespace SemVer {

  struct Version;

  class VersionRange {
  public:
    VersionRange(std::initializer_list<Version>);
    bool matches(Version const&) const;
  };

} // SemVer