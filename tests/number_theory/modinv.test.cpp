#include "number_theory/modinv.hpp"

#include <array>
#include <cassert>
#include <limits>
#include <numeric>

int main() {
  assert(modinv(3, 11) == 4);
  assert(modinv(-3, 11) == 7);
  assert(modinv(17, 11) == 2);
  assert(modinv(-17, 11) == 9);
  assert(modinv(3, 8) == 3);
  assert(modinv(7, 15) == 13);
  assert(modinv(-7, 15) == 2);
  assert(modinv(0, 1) == 0);
  assert(modinv(123, 1) == 0);
  assert(modinv(-123, 1) == 0);

  // LLONG_MIN is outside the supported input domain.
  const long long lmax = std::numeric_limits<long long>::max();
  const int imax = std::numeric_limits<int>::max();
  const std::array values = {-lmax,
                             -lmax + 1,
                             -1LL * imax - 1,
                             -1LL * imax,
                             -17LL,
                             -3LL,
                             -1LL,
                             0LL,
                             1LL,
                             3LL,
                             17LL,
                             1LL * imax,
                             1LL * imax + 1,
                             lmax - 1,
                             lmax};
  const std::array moduli = {
      1, 2, 3, 8, 12, 15, 998'244'353, 1'000'000'007, imax - 1, imax};
  for (long long a: values) {
    for (int m: moduli) {
      if (std::gcd(a, m) != 1) continue;
      const long long x = modinv(a, m);
      assert(0 <= x && x < m);
      // Reduce a first: a * x may overflow long long.
      long long r = a % m;
      if (r < 0) r += m;
      assert(r * x % m == 1 % m);
    }
  }

  // Compare exact results for large moduli: even (a % m) * x may overflow.
  assert(modinv(1, lmax) == 1);
  assert(modinv(-1, lmax) == lmax - 1);
  assert(modinv(2, lmax) == lmax / 2 + 1);
  assert(modinv(-2, lmax) == lmax / 2);
  assert(modinv(lmax - 1, lmax) == lmax - 1);
  assert(modinv(1 - lmax, lmax) == 1);
  assert(modinv(lmax, lmax - 1) == 1);
  assert(modinv(-lmax, lmax - 1) == lmax - 2);

  // Consecutive Fibonacci numbers exercise many Euclidean algorithm steps.
  const long long f91 = 4'660'046'610'375'530'309;
  const long long f92 = 7'540'113'804'746'346'429;
  assert(modinv(f91, f92) == f91);
  assert(modinv(-f91, f92) == f92 - f91);
}
