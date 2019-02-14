#include <catch2/catch.hpp>
#include "SemVer/Version.h"

TEST_CASE("Semantic versions can be compared") {
  SemVer::Version lower_precedent{1, 0, 0};
  SemVer::Version higher_precedent{2, 0, 0};
  REQUIRE(lower_precedent < higher_precedent);

  SECTION("the opposite must be false") {
    REQUIRE_FALSE(higher_precedent < lower_precedent);
  }
}