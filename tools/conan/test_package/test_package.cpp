#include "SemVer/Version.h"


auto main(int, const char*[]) -> int {
  auto const v = SemVer::From("1.2.3-alpha.1");
  return v.patch - 3; // Must return 0 for successful testing
}