#include "SemVer/VersionRange.h"
#include "SemVer/Version.h"

namespace {
  bool SemanticEqual(
      SemVer::Version const& lhs, SemVer::Version const& rhs) noexcept {
    return lhs.major == rhs.major
        && lhs.minor == rhs.minor
        && lhs.patch == rhs.patch
        && lhs.prerelease == rhs.prerelease
    ;
  }
}

namespace SemVer {

  VersionRange::VersionRange(std::initializer_list<Version> versions)
  : set_(versions) {}

  bool VersionRange::matches(Version const& version) const {
    return set_.count(version) > 0;
  }

  bool Check(Version const& version, Comparator const& against) noexcept {
    switch(against.type) {
      case Comparator::Type::Greater: return against.operand < version;
      case Comparator::Type::GreaterEqual: return against.operand <= version;
      case Comparator::Type::Less: return version < against.operand;
      case Comparator::Type::LessEqual: return version <= against.operand;
      case Comparator::Type::Equal: return SemanticEqual(version, against.operand);
      case Comparator::Type::NotEqual: return !SemanticEqual(version, against.operand);
    }
  }

} // SemVer