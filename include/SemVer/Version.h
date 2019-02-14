#pragma once

#include <ostream>
#include <string>

namespace SemVer {
  struct Version {
    int const major;
    int const minor;
    int const patch;

    using Meta = std::string;
    Meta const prerelease; 
    Meta const build;

    std::string to_string() const;
  };

  bool operator<(Version const&, Version const&) noexcept;
  bool operator==(Version const&, Version const&) noexcept;
  bool operator!=(Version const&, Version const&) noexcept;
  std::ostream& operator<<(std::ostream&, Version const&);
} // SemVer