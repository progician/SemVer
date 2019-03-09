#include <catch2/catch.hpp>
#include "SemVer/Version.h"
#include "SemVer/VersionRange.h"

TEST_CASE("Comparators operators against constant versions at runtime") {
  SECTION("for greater-than") {
    SemVer::Comparator const gt{
        SemVer::Comparator::Type::Greater,
        SemVer::Version{1, 0, 0}
    };
    REQUIRE(SemVer::Check(SemVer::Version{2, 0, 0}, gt));
    REQUIRE_FALSE(SemVer::Check(SemVer::Version{0, 1, 0}, gt));
  }
}

TEST_CASE("Version ranges can define a list of acceptable versions") {
  SemVer::VersionRange const range{
    {1, 0, 0},
    {1, 3, 0},
    {1, 5, 0},
  };

  REQUIRE(range.matches({1, 3, 0}));
  REQUIRE_FALSE(range.matches({2, 0, 0}));
}