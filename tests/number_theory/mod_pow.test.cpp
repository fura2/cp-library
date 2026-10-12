#include "number_theory/mod_pow.hpp"

#include <cassert>
#include <limits>

int main() {
  assert(mod_pow(2, 10, 1000) == 24);
  assert(mod_pow(-2, 3, 7) == 6);
  assert(mod_pow(-2, 4, 7) == 2);
  assert(mod_pow(0, 5, 13) == 0);
  assert(mod_pow(0, 0, 13) == 1);
  assert(mod_pow(123, 0, 1) == 0);
  assert(mod_pow(123, 100, 1) == 0);
  assert(mod_pow(1, std::numeric_limits<long long>::max(), 7) == 1);
  assert(mod_pow(2'147'483'646LL, 2, 2'147'483'647) == 1);
}
