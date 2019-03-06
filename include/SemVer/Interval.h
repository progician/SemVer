#pragma once

#include "SemVer/Version.h"

namespace SemVer {
  /// Inclusive interval range type
  struct Interval {
    Version const begin;
    Version const end;
  };

  bool Match(Interval const&, Version const&) noexcept;
} // SemVer