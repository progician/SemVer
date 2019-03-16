#include "SemVer/VersionRange.h"
#include "SemVer/Version.h"

#include <algorithm>
#include <sstream>

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
      if (str[0] == '<') {
        if (str[1] == '=') {
          return {Comparator::Type::LessEqual, From(str.substr(2))};
        }
        return {Comparator::Type::Less, From(str.substr(1))};
      }
      return {Comparator::Type::Equal, From(str)};
    }

    ComparatorSet ComparatorSetFrom(std::string str) {
      ComparatorSet result;
      std::istringstream sstr(std::move(str));
      std::transform(
          std::istream_iterator<std::string>(sstr),
          std::istream_iterator<std::string>(),
          std::back_inserter(result),
          ComparatorFrom
      );
      return result;
    }


    std::vector<std::string> Split(std::string str, std::string const& delim) {
      std::vector<std::string> result;
      std::string::size_type lp = 0;
      std::string::size_type pos = std::string::npos;
      for (
          pos = str.find(delim);
          pos != std::string::npos;
          lp = pos + delim.size(), pos = str.find(delim, lp)
      ) {
        std::string const part = str.substr(lp, pos - lp);
        result.push_back(part);
      }
      result.push_back(str.substr(lp));
      return result;
    }
  }


  VersionRange RangeFrom(std::string const& str) {
    auto const parts = Split(str, "||");
    VersionRange version_range;
    std::transform(
        std::begin(parts), std::end(parts),
        std::back_inserter(version_range),
        ComparatorSetFrom
    );
    return version_range;
  }
} // SemVer