#include "SemVer/VersionRange.h"
#include "SemVer/Version.h"

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
      case Comparator::Type::Equal: return version == against.operand;
      case Comparator::Type::NotEqual: return version != against.operand;
    }
  }

} // SemVer