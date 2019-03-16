#include <catch2/catch.hpp>
#include "SemVer/Version.h"
#include "SemVer/VersionRange.h"

TEST_CASE("Comparators operators against constant versions at runtime") {
  SECTION("for greater-than") {
    SemVer::Comparator const GreaterThan{
        SemVer::Comparator::Type::Greater,
        SemVer::Version{1, 0, 0}
    };
    REQUIRE(SemVer::Match(SemVer::Version{2, 0, 0}, GreaterThan));
    REQUIRE_FALSE(SemVer::Match(SemVer::Version{0, 1, 0}, GreaterThan));
    REQUIRE_FALSE(SemVer::Match(SemVer::Version{1, 0, 0}, GreaterThan));
  }

  SECTION("for greater-or-equal") {
    SemVer::Comparator const GreaterEqual{
        SemVer::Comparator::Type::GreaterEqual,
        SemVer::Version{1, 0, 0}
    };
    REQUIRE(SemVer::Match(SemVer::Version{1, 0, 0}, GreaterEqual));
    REQUIRE(SemVer::Match(SemVer::Version{2, 0, 0}, GreaterEqual));
    REQUIRE_FALSE(SemVer::Match(SemVer::Version{0, 1, 0}, GreaterEqual));
  }

  SECTION("for less-than") {
    SemVer::Comparator const LessThan{
        SemVer::Comparator::Type::Less,
        SemVer::Version{3, 2, 1}
    };
    REQUIRE(SemVer::Match(SemVer::Version{1, 0, 0}, LessThan));
    REQUIRE(SemVer::Match(SemVer::Version{3, 2, 1, "alpha"}, LessThan));
    REQUIRE_FALSE(SemVer::Match(SemVer::Version{3, 2, 1}, LessThan));
    REQUIRE_FALSE(SemVer::Match(SemVer::Version{4, 0, 0}, LessThan));
  }

  SECTION("for less-equal") {
    SemVer::Comparator const LessOrEqual{
        SemVer::Comparator::Type::LessEqual,
        SemVer::Version{3, 2, 1}
    };
    REQUIRE(SemVer::Match(SemVer::Version{1, 0, 0}, LessOrEqual));
    REQUIRE(SemVer::Match(SemVer::Version{3, 2, 1}, LessOrEqual));
    REQUIRE_FALSE(SemVer::Match(SemVer::Version{4, 0, 0}, LessOrEqual));
  }

  SECTION("for equal") {
    SemVer::Comparator const Equal{
        SemVer::Comparator::Type::Equal,
        SemVer::Version{3, 2, 1}
    };
    REQUIRE(SemVer::Match(SemVer::Version{3, 2, 1}, Equal));
    REQUIRE_FALSE(SemVer::Match(SemVer::Version{4, 0, 0}, Equal));

    SECTION("build meta-data is not considered for equality") {
      REQUIRE(
          SemVer::Match(SemVer::Version{3, 2, 1, "", "buildmeta"}, Equal)
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
  REQUIRE(SemVer::Match(SemVer::Version{1, 5}, Interval));
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
  REQUIRE(SemVer::Match(SemVer::Version{1, 5}, SimpleVersionRange));
  REQUIRE_FALSE(SemVer::Match(SemVer::Version{2, 5}, SimpleVersionRange));
  REQUIRE(SemVer::Match(SemVer::Version{3, 5}, SimpleVersionRange));
}


TEST_CASE("Version ranges can be parsed up from strings by the following simple"
          " rules"
) {
  SECTION("greater-than denoted with >VERSION") {
    auto const over_one = SemVer::RangeFrom(">1.0.0");
    REQUIRE(SemVer::Match(SemVer::Version{1, 1, 0}, over_one));
    REQUIRE_FALSE(SemVer::Match(SemVer::Version{0, 1, 0}, over_one));
    REQUIRE_FALSE(SemVer::Match(SemVer::Version{1, 0, 0}, over_one));
  }

  SECTION("greater-equal denoted with >=VERSION") {
    auto const greater_equal_one = SemVer::RangeFrom(">=1.0.0");
    REQUIRE(SemVer::Match(SemVer::Version{1, 1, 0}, greater_equal_one));
    REQUIRE_FALSE(SemVer::Match(SemVer::Version{0, 1, 0}, greater_equal_one));
    REQUIRE(SemVer::Match(SemVer::Version{1, 0, 0}, greater_equal_one));
  }

  SECTION("equality denoted with =VERSION or just VERSION") {
    REQUIRE(SemVer::Match(SemVer::Version{1, 2, 3}, SemVer::RangeFrom("=1.2.3")));
    REQUIRE(SemVer::Match(SemVer::Version{2, 3, 1}, SemVer::RangeFrom("2.3.1")));
  }

  SECTION("less-than denoted with <VERSION") {
    auto const under_three = SemVer::RangeFrom("<3.0.0");
    REQUIRE(SemVer::Match(SemVer::Version{1, 1, 0}, under_three));
    REQUIRE_FALSE(SemVer::Match(SemVer::Version{10, 1, 0}, under_three));
    REQUIRE_FALSE(SemVer::Match(SemVer::Version{3, 0, 0}, under_three));
  }

  SECTION("less-equal denoted with <=VERSION") {
    auto const under_three = SemVer::RangeFrom("<=3.0.0");
    REQUIRE(SemVer::Match(SemVer::Version{1, 1, 0}, under_three));
    REQUIRE_FALSE(SemVer::Match(SemVer::Version{10, 1, 0}, under_three));
    REQUIRE(SemVer::Match(SemVer::Version{3, 0, 0}, under_three));
  }
}

TEST_CASE("Version ranges expressions separated by spaces are forming a"
          " comparator set."
) {
  auto const one_to_three = SemVer::RangeFrom(">=1.0.0 <=3.0.0");
  REQUIRE(SemVer::Match(SemVer::From("1.0.0"), one_to_three));
  REQUIRE(SemVer::Match(SemVer::From("1.5.0"), one_to_three));
  REQUIRE(SemVer::Match(SemVer::From("3.0.0"), one_to_three));
  REQUIRE_FALSE(SemVer::Match(SemVer::From("0.9.0"), one_to_three));
  REQUIRE_FALSE(SemVer::Match(SemVer::From("3.9.0"), one_to_three));
}

TEST_CASE("Range can be composed of multiple comparator sets delimited by ||") {
  auto const one_or_three = SemVer::RangeFrom("=1.0.0||=3.0.0");
  REQUIRE(SemVer::Match(SemVer::From("1.0.0"), one_or_three));
  REQUIRE_FALSE(SemVer::Match(SemVer::From("1.5.0"), one_or_three));
  REQUIRE(SemVer::Match(SemVer::From("3.0.0"), one_or_three));
}


TEST_CASE("The || delimiter in version ranges can be surrounded with whitespace") {
  auto const one_or_three = SemVer::RangeFrom("=1.0.0\t       ||\t\t    \t=3.0.0");
  REQUIRE(SemVer::Match(SemVer::From("1.0.0"), one_or_three));
  REQUIRE_FALSE(SemVer::Match(SemVer::From("1.5.0"), one_or_three));
  REQUIRE(SemVer::Match(SemVer::From("3.0.0"), one_or_three));
}