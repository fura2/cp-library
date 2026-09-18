#include "number_theory/extended_gcd.hpp"

#include <array>
#include <cassert>
#include <cstdlib>
#include <limits>
#include <numeric>
#include <tuple>

int main() {
  assert((extended_gcd(5, 3) == std::tuple{1, 2, -3}));
  assert((extended_gcd(5, -3) == std::tuple{1, 2, 3}));
  assert((extended_gcd(-5, 3) == std::tuple{1, 1, 2}));
  assert((extended_gcd(-5, -3) == std::tuple{1, 1, -2}));
  assert((extended_gcd(-3, 5) == std::tuple{1, 3, 2}));
  assert((extended_gcd(12, 18) == std::tuple{6, 2, -1}));
  assert((extended_gcd(0, 0) == std::tuple{0, 0, 0}));

  // The minimum signed value is outside the supported input domain.
  const int imax = std::numeric_limits<int>::max();
  const std::array values = {-imax,
                             -imax + 1,
                             -30,
                             -12,
                             -5,
                             -3,
                             -2,
                             -1,
                             0,
                             1,
                             2,
                             3,
                             5,
                             12,
                             30,
                             1'134'903'170,
                             1'836'311'903,
                             imax - 1,
                             imax};
  for (int a: values) {
    for (int b: values) {
      const auto [g, x, y] = extended_gcd(a, b);
      assert(g == std::gcd(a, b));
      assert(1LL * a * x + 1LL * b * y == g);
      if (b == 0) {
        assert(x == (a > 0) - (a < 0));
        assert(y == 0);
      }
      else {
        assert(0 <= x && x < std::abs(b) / g);
        if (a == 0) {
          assert(y == (b > 0) - (b < 0));
        }
        else {
          assert(std::abs(1LL * y) <= std::abs(1LL * a) / g);
        }
      }
    }
  }

  // Compare exact results here: a*x and b*y may overflow long long.
  const long long m = std::numeric_limits<long long>::max();
  assert((extended_gcd(0LL, 0LL) == std::tuple{0LL, 0LL, 0LL}));
  assert((extended_gcd(m, 0LL) == std::tuple{m, 1LL, 0LL}));
  assert((extended_gcd(-m, 0LL) == std::tuple{m, -1LL, 0LL}));
  assert((extended_gcd(0LL, m) == std::tuple{m, 0LL, 1LL}));
  assert((extended_gcd(0LL, -m) == std::tuple{m, 0LL, -1LL}));
  assert((extended_gcd(m, m) == std::tuple{m, 0LL, 1LL}));
  assert((extended_gcd(-m, -m) == std::tuple{m, 0LL, -1LL}));
  assert((extended_gcd(m, m - 1) == std::tuple{1LL, 1LL, -1LL}));
  assert((extended_gcd(m, 1 - m) == std::tuple{1LL, 1LL, 1LL}));
  assert((extended_gcd(-m, m - 1) == std::tuple{1LL, m - 2, m - 1}));
  assert((extended_gcd(-m, 1 - m) == std::tuple{1LL, m - 2, 1 - m}));
  assert((extended_gcd(m - 2, -3LL) == std::tuple{1LL, 2LL, 2 * (m / 3) - 1}));
}
