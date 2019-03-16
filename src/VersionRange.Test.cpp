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
}


TEST_CASE("Comparators can be composed into comparator sets"
          "which joined by intersection") {
  using Type = SemVer::Comparator::Type;
  SemVer::ComparatorSet const Interval{
      SemVer::Comparator{Type::Greater, SemVer::Version{1, 0, 0}},
      SemVer::Comparator{Type::LessEqual, SemVer::Version{2, 0, 0}},
  };
  REQUIRE(SemVer::Check(SemVer::Version{1, 5}, Interval));
}


TEST_CASE("Version ranges composed of one or more comparator sets") {
  using Type = SemVer::Comparator::Type;
  SemVer::VersionRange const SimpleVersionRange{
      SemVer::ComparatorSet{
          SemVer::Comparator{Type::Greater, SemVer::Version{1, 0, 0}},
          SemVer::Comparator{Type::LessEqual, SemVer::Version{2, 0, 0}},
      },
      SemVer::ComparatorSet{
          SemVer::Comparator{Type::Greater, SemVer::Version{3, 0, 0}},
          SemVer::Comparator{Type::LessEqual, SemVer::Version{4, 0, 0}},
      },
  };
  REQUIRE(SemVer::Check(SemVer::Version{1, 5}, SimpleVersionRange));
  REQUIRE_FALSE(SemVer::Check(SemVer::Version{2, 5}, SimpleVersionRange));
  REQUIRE(SemVer::Check(SemVer::Version{3, 5}, SimpleVersionRange));
}