#include "number_theory/modpow.hpp"

#include <cassert>
#include <limits>

int main() {
  assert(modpow(2, 10, 1000) == 24);
  assert(modpow(-2, 3, 7) == 6);
  assert(modpow(-2, 4, 7) == 2);
  assert(modpow(0, 5, 13) == 0);
  assert(modpow(0, 0, 13) == 1);
  assert(modpow(123, 0, 1) == 0);
  assert(modpow(123, 100, 1) == 0);
  assert(modpow(1, std::numeric_limits<long long>::max(), 7) == 1);
  assert(modpow(2'147'483'646LL, 2, 2'147'483'647) == 1);
}
