#pragma once

#include "SemVer/Version.h"

#include <utility>
#include <vector>

namespace SemVer {

  /// The most basic unit of a version range. It represents a single version and
  /// relation to that version.
  struct Comparator {
    enum class Type {
      Greater,
      GreaterEqual,
      Less,
      LessEqual,
      Equal,
    };
    Type const type;
    Version const operand;

    Comparator(Type t, Version const& op) : type{t}, operand{op} {}

    /// The pre-release and build meta are strings, which might be better moved.
    Comparator(Type t, Version&& op)
    : type{t}, operand{std::forward<Version>(op)} {}
  };

  /// Comparators with 'AND' as in intersection relationship between them.
  using ComparatorSet = std::vector<Comparator>;
  /// ComparatorSets with 'OR' as in union relationship between them.
  using VersionRange = std::vector<ComparatorSet>;

  auto Match(Version const&, Comparator const&) noexcept -> bool;
  auto Match(Version const&, ComparatorSet const&) noexcept -> bool;
  auto Match(Version const&, VersionRange const&) noexcept -> bool;

  /// Parses up the string into a version range object. If the expression is not
  /// valid, it throws a std::invalid_argument exception.
  auto RangeFrom(std::string const&) -> VersionRange;
} // SemVer