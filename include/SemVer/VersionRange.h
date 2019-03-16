#pragma once

#include "SemVer/Version.h"

#include <initializer_list>
#include <set>
#include <utility>
#include <vector>

namespace SemVer {

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

    Comparator(Type t, Version&& op)
    : type{t}, operand{std::forward<Version>(op)} {}
  };
  using ComparatorSet = std::vector<Comparator>;
  using VersionRange = std::vector<ComparatorSet>;

  bool Check(Version const&, Comparator const&) noexcept;
  bool Check(Version const&, ComparatorSet const&) noexcept;
  bool Check(Version const&, VersionRange const&) noexcept;
} // SemVer