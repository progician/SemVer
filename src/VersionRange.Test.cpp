#include <catch2/catch.hpp>
#include "SemVer/Version.h"
#include "SemVer/VersionRange.h"

TEST_CASE("Version ranges can define a list of acceptable versions") {
  SemVer::VersionRange const range{
    {1, 0, 0},
    {1, 3, 0},
    {1, 5, 0},
  };

  REQUIRE(range.matches({1, 3, 0}));
  REQUIRE_FALSE(range.matches({2, 0, 0}));
}