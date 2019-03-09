#include "SemVer/VersionRange.h"
#include "SemVer/Version.h"

namespace SemVer {

  VersionRange::VersionRange(std::initializer_list<Version> versions)
  : set_(versions) {}

  bool VersionRange::matches(Version const& version) const {
    return set_.count(version) > 0;
  }

  bool Check(Version const& version, Comparator const& against) noexcept {
    return against.operand < version;
  }

} // SemVer