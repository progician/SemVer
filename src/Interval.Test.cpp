#include <catch2/catch.hpp>
#include "SemVer/Interval.h"

TEST_CASE("Intervals are inclusive range that match with") {
  SemVer::Interval const arbitraryInterval{
    SemVer::Version{1, 0, 0}, SemVer::Version{2, 0, 0}
  };

  SECTION("anything entirely within the range") {
    REQUIRE(SemVer::Match(arbitraryInterval, SemVer::Version{1, 2, 3}));
  }

  SECTION("the beginning of the range") {
    REQUIRE(SemVer::Match(arbitraryInterval, arbitraryInterval.begin));
  }

  SECTION("the end of the range") {
    REQUIRE(SemVer::Match(arbitraryInterval, arbitraryInterval.end));
  }
}
