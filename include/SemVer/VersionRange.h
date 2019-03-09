#pragma once

#include "SemVer/Version.h"

#include <initializer_list>
#include <set>
#include <utility>

namespace SemVer {

  struct Comparator {
    enum class Type {
      Greater,
      GreaterEqual,
      Less,
      LessEqual,
      Equal,
      NotEqual,
    };

    Type const type;
    Version const operand;

    Comparator(Type t, Version const& op) : type{t}, operand{op} {}

    Comparator(Type t, Version&& op)
    : type{t}, operand{std::forward<Version>(op)} {}
  };
  bool Check(Version const&, Comparator const&) noexcept;

  class VersionRange {
    std::set<Version> set_;

  public:
    VersionRange(std::initializer_list<Version>);
    bool matches(Version const&) const;
  };

} // SemVer