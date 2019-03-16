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

  SECTION("non-equality is if any the main digits are different") {
    SemVer::Version const arbitrary_version{1, 2, 3};
    SemVer::Version const different_digits{3, 2, 1};
    REQUIRE(arbitrary_version != different_digits);
  }

  SECTION("equality operator takes pre-release and build metadata in account") {
    SemVer::Version const prereleased_some{1, 2, 3, "some"};
    SemVer::Version const prereleased_other{1, 2, 3, "other"};
    REQUIRE_FALSE(prereleased_some == prereleased_other);

    SemVer::Version const build_meta_added{1, 2, 3, "some", "build_numba"};
    REQUIRE_FALSE(prereleased_some == build_meta_added);
  }
}

TEST_CASE("Printing rules for version objects") {
  SECTION("always include the three main digits") {
    SemVer::Version only_digits{1, 2, 3};
    REQUIRE(only_digits.to_string() == "1.2.3");
  }

  SECTION("prerelease is hyphenated") {
    SemVer::Version with_prerelease{1, 2, 3, "pre"};
    REQUIRE(with_prerelease.to_string() == "1.2.3-pre");
  }

  SECTION("build meta is with prefixed by + and comes immediately without prerelease") {
    SemVer::Version with_buildmeta{1, 2, 3, "", "build_numba"};
    REQUIRE(with_buildmeta.to_string() == "1.2.3+build_numba");
  }

  SECTION("build meta comes after the pre is exists") {
    SemVer::Version with_pre_and_meta{1, 2, 3, "alpha", "build_numba"};
    REQUIRE(with_pre_and_meta.to_string() == "1.2.3-alpha+build_numba");
  }
}

TEST_CASE("Metadata role in version precedence") {
  SECTION("pre-release meta data") {
    SemVer::Version alpha_prerelease{1, 2, 3, "alpha"};
    SECTION("takes precedence by ascending alphabetic order of identifier") {
      REQUIRE(alpha_prerelease < SemVer::Version{1, 2, 3, "beta"});
    }
  }
}


TEST_CASE("Version object can be parsed up from strings") {
  REQUIRE(SemVer::From("0.0.4") == SemVer::Version{0, 0, 4});
  REQUIRE(SemVer::From("1.2.3") == SemVer::Version{1, 2, 3});
  REQUIRE(SemVer::From("10.20.30") == SemVer::Version{10, 20, 30});

  REQUIRE(SemVer::From("1") == SemVer::Version{1, 0, 0});
  REQUIRE(SemVer::From("1.2") == SemVer::Version{1, 2, 0});

  REQUIRE(SemVer::From("1.1.2-prerelease") == SemVer::Version{1, 1, 2, "prerelease"});
}