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

  bool Match(Version const&, Comparator const&) noexcept;
  bool Match(Version const&, ComparatorSet const&) noexcept;
  bool Match(Version const&, VersionRange const&) noexcept;
} // SemVer