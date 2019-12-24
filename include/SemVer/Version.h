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

    Version(int maj, int min) : major{maj}, minor{min}, patch{0} {}
    Version(int maj, int min, int p) : major{maj}, minor{min}, patch{p} {}
    
    Version(int maj, int min, int p, Meta pr)
        : major{maj}
        , minor{min}
        , patch{p}
        , prerelease{std::move(pr)} {}

    Version(int maj, int min, int p, Meta pr, Meta b)
        : major{maj}
        , minor{min}
        , patch{p}
        , prerelease{std::move(pr)}
        , build{std::move(b)} {}
  };

  bool operator<(Version const&, Version const&) noexcept;
  bool operator==(Version const&, Version const&) noexcept;
  bool operator<=(Version const&, Version const&) noexcept;
  bool operator!=(Version const&, Version const&) noexcept;
  std::ostream& operator<<(std::ostream&, Version const&);
  std::string to_string(Version const&);

  /// Parsing up a string from a string. Failing to do so, throws
  /// runtime exception
  Version From(std::string const&);
} // SemVer