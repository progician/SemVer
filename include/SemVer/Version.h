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

  auto operator<(Version const&, Version const&) noexcept -> bool;
  auto operator==(Version const&, Version const&) noexcept -> bool;
  auto operator<=(Version const&, Version const&) noexcept -> bool;
  auto operator!=(Version const&, Version const&) noexcept -> bool;
  auto operator<<(std::ostream&, Version const&) -> std::ostream&;
  auto to_string(Version const&) -> std::string;

  /// Parsing up a string from a string. Failing to do so, throws
  /// runtime exception
  auto From(std::string const&) -> Version;
} // SemVer