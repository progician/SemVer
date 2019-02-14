#include <catch2/catch.hpp>
#include "SemVer/Version.h"

TEST_CASE("Semantic versions can be compared") {
  SemVer::Version const lower_precedent{1, 0, 0};
  SemVer::Version const higher_precedent{2, 0, 0};
  REQUIRE(lower_precedent < higher_precedent);

  SECTION("the opposite must be false") {
    REQUIRE_FALSE(higher_precedent < lower_precedent);
  }

  SECTION("equality is if all the main digits are equal") {
    SemVer::Version const one_of_the_same{1, 2, 3};
    SemVer::Version const another_one_of_the_same{1, 2, 3};
    REQUIRE(one_of_the_same == another_one_of_the_same);
  }
}