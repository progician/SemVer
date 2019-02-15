#include "SemVer/VersionRange.h"
#include "SemVer/Version.h"

namespace SemVer {

  VersionRange::VersionRange(std::initializer_list<Version>)
  {}

  bool VersionRange::matches(Version const&) const {
    return true;
  }

} // SemVer