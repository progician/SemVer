#include <catch2/catch.hpp>
#include "SemVer/Version.h"
#include "SemVer/VersionRange.h"

TEST_CASE("Version ranges can define a list of acceptable versions") {
  SemVer::VersionRange const range{
    SemVer::Version{1, 0, 0},
    SemVer::Version{1, 3, 0},
    SemVer::Version{1, 5, 0},
  };

  REQUIRE(range.matches(SemVer::Version{1, 3, 0}));
}