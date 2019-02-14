#pragma once

#include <ostream>

namespace SemVer {
  struct Version {
    int const major;
    int const minor;
    int const patch;
  };

  bool operator<(Version const&, Version const&) noexcept;
  bool operator==(Version const&, Version const&) noexcept;
  bool operator!=(Version const&, Version const&) noexcept;
  std::ostream& operator<<(std::ostream&, Version const&);
} // SemVer