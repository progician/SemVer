#include "SemVer/VersionRange.h"
#include "SemVer/Version.h"

#include <algorithm>

namespace {
  bool SemanticEqual(
      SemVer::Version const& lhs,
      SemVer::Version const& rhs
  ) noexcept {
    return lhs.major == rhs.major
        && lhs.minor == rhs.minor
        && lhs.patch == rhs.patch
        && lhs.prerelease == rhs.prerelease
    ;
  }
}

namespace SemVer {
  bool Match(Version const& version, Comparator const& against) noexcept {
    switch(against.type) {
      case Comparator::Type::Greater: return against.operand < version;
      case Comparator::Type::GreaterEqual: return against.operand <= version;
      case Comparator::Type::Less: return version < against.operand;
      case Comparator::Type::LessEqual: return version <= against.operand;
      case Comparator::Type::Equal: return SemanticEqual(version, against.operand);
    }
  }


  bool Match(Version const& version, ComparatorSet const& against) noexcept {
    return std::all_of(
        std::begin(against), std::end(against),
        [&version](auto const& comparator) { return Match(version, comparator); }
    );
  }


  bool Match(Version const& version, VersionRange const& against) noexcept {
    return std::any_of(
        std::begin(against), std::end(against),
        [&version](auto const& cset) { return Match(version, cset); }
    );
  }


  namespace {
    Comparator ComparatorFrom(std::string const& str) {
      if (str[0] == '>') {
        if (str[1] == '=') {
          return {Comparator::Type::GreaterEqual, From(str.substr(2))};
        }
        return {Comparator::Type::Greater, From(str.substr(1))};
      }
      else if (str[0] == '=') {
        return {Comparator::Type::Equal, From(str.substr(1))};
      }

      throw std::invalid_argument{"unable to parse version range expression"};
    }
  }


  VersionRange RangeFrom(std::string const& str) {
    return {{ComparatorFrom(str)}};
  }
} // SemVer