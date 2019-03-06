#include "SemVer/Interval.h"

namespace SemVer {
  bool Match(Interval const& interval, Version const& version) noexcept {
    return interval.begin <= version && version <= interval.end;
  }
} // SemVer
