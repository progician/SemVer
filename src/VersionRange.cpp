#include "SemVer/VersionRange.h"
#include "SemVer/Version.h"

#include <algorithm>

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
  bool Check(Version const& version, Comparator const& against) noexcept {
    switch(against.type) {
      case Comparator::Type::Greater: return against.operand < version;
      case Comparator::Type::GreaterEqual: return against.operand <= version;
      case Comparator::Type::Less: return version < against.operand;
      case Comparator::Type::LessEqual: return version <= against.operand;
      case Comparator::Type::Equal: return SemanticEqual(version, against.operand);
    }
  }


  bool Check(Version const& version, ComparatorSet const& against) noexcept {
    return std::all_of(
        std::begin(against), std::end(against),
        [&version](auto const& comparator) { return Check(version, comparator); }
    );
  }


  bool Check(Version const& version, VersionRange const& against) noexcept {
    return std::any_of(
        std::begin(against), std::end(against),
        [&version](auto const& cset) { return Check(version, cset); }
    );
  }
} // SemVer