#include <catch2/catch.hpp>
#include "SemVer/Version.h"
#include "SemVer/VersionRange.h"

TEST_CASE("Comparators operators against constant versions at runtime") {
  SECTION("for greater-than") {
    SemVer::Comparator const GreaterThan{
        SemVer::Comparator::Type::Greater,
        SemVer::Version{1, 0, 0}
    };
    REQUIRE(SemVer::Check(SemVer::Version{2, 0, 0}, GreaterThan));
    REQUIRE_FALSE(SemVer::Check(SemVer::Version{0, 1, 0}, GreaterThan));
    REQUIRE_FALSE(SemVer::Check(SemVer::Version{1, 0, 0}, GreaterThan));
  }

  SECTION("for greater-or-equal") {
    SemVer::Comparator const GreaterEqual{
        SemVer::Comparator::Type::GreaterEqual,
        SemVer::Version{1, 0, 0}
    };
    REQUIRE(SemVer::Check(SemVer::Version{1, 0, 0}, GreaterEqual));
    REQUIRE(SemVer::Check(SemVer::Version{2, 0, 0}, GreaterEqual));
    REQUIRE_FALSE(SemVer::Check(SemVer::Version{0, 1, 0}, GreaterEqual));
  }

  SECTION("for less-than") {
    SemVer::Comparator const LessThan{
        SemVer::Comparator::Type::Less,
        SemVer::Version{3, 2, 1}
    };
    REQUIRE(SemVer::Check(SemVer::Version{1, 0, 0}, LessThan));
    REQUIRE(SemVer::Check(SemVer::Version{3, 2, 1, "alpha"}, LessThan));
    REQUIRE_FALSE(SemVer::Check(SemVer::Version{3, 2, 1}, LessThan));
    REQUIRE_FALSE(SemVer::Check(SemVer::Version{4, 0, 0}, LessThan));
  }

  SECTION("for less-equal") {
    SemVer::Comparator const LessOrEqual{
        SemVer::Comparator::Type::LessEqual,
        SemVer::Version{3, 2, 1}
    };
    REQUIRE(SemVer::Check(SemVer::Version{1, 0, 0}, LessOrEqual));
    REQUIRE(SemVer::Check(SemVer::Version{3, 2, 1}, LessOrEqual));
    REQUIRE_FALSE(SemVer::Check(SemVer::Version{4, 0, 0}, LessOrEqual));
  }

  SECTION("for equal") {
    SemVer::Comparator const Equal{
        SemVer::Comparator::Type::Equal,
        SemVer::Version{3, 2, 1}
    };
    REQUIRE(SemVer::Check(SemVer::Version{3, 2, 1}, Equal));
    REQUIRE_FALSE(SemVer::Check(SemVer::Version{4, 0, 0}, Equal));

    SECTION("build meta-data is not considered for equality") {
      REQUIRE(
          SemVer::Check(SemVer::Version{3, 2, 1, "", "buildmeta"}, Equal)
      );
    }
  }

  SECTION("for not-equal") {
    SemVer::Comparator const NotEqual{
        SemVer::Comparator::Type::NotEqual,
        SemVer::Version{3, 2, 1}
    };
    REQUIRE_FALSE(SemVer::Check(SemVer::Version{3, 2, 1}, NotEqual));
    REQUIRE(SemVer::Check(SemVer::Version{4, 0, 0}, NotEqual));

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