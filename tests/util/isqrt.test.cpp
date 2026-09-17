#include "util/isqrt.hpp"

#include <cassert>
#include <limits>

int main() {
  assert(isqrt(0) == 0);
  assert(isqrt(1) == 1);
  assert(isqrt(2) == 1);
  assert(isqrt(15) == 3);
  assert(isqrt(16) == 4);
  assert(isqrt(17) == 4);
  assert(isqrt(999'999'999'999'999'999LL) == 999'999'999);
  assert(isqrt(1'000'000'000'000'000'000LL) == 1'000'000'000);
  assert(isqrt(std::numeric_limits<long long>::max()) == 3'037'000'499LL);
}
