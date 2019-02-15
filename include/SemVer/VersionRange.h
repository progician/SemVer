#pragma once

#include "SemVer/Version.h"

#include <initializer_list>
#include <set>

namespace SemVer {

  class VersionRange {
    std::set<Version> set_;

  public:
    VersionRange(std::initializer_list<Version>);
    bool matches(Version const&) const;
  };

} // SemVer