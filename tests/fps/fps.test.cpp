#include "fps/fps.hpp"

#include <cassert>
#include <initializer_list>
#include <utility>
#include <vector>

void check(const fps& f, std::initializer_list<int> expected) {
  int i = 0;
  for (int x: expected) assert(f[i++].unwrap() == mint{x}.unwrap());
}

int main() {
  const fps zero{std::vector<int>{}, 4};
  assert(zero.precision() == 4);
  check(zero, {0, 0, 0, 0});
  const fps a({1, 2, 3}, 5), b({4, 5}, 3);
  check(a + b, {5, 7, 3});
  check(a - b, {-3, -3, 3});
  check(a * b, {4, 13, 22});
  assert((a + b).precision() == 3);
  assert((a - b).precision() == 3);
  assert((a * b).precision() == 3);
  check(a * zero, {0, 0, 0, 0});
  check(zero * a, {0, 0, 0, 0});

  fps f(4);
  f[2] = 7;
  check(f, {0, 0, 7, 0});
  f += b;
  check(f, {4, 5, 7});
  assert(f.precision() == 3);
  f -= b;
  check(f, {0, 0, 7});
  f *= fps({2}, 2);
  assert(f.precision() == 2);
  check(f, {0, 0});

  fps self({1, 2}, 4);
  self *= self;
  check(self, {1, 4, 4, 0});
  self += self;
  check(self, {2, 8, 8, 0});
  self -= self;
  check(self, {0, 0, 0, 0});

  const std::vector<int> v = {1, 2, 3};
  check(fps(v, 2), {1, 2});
  check(fps(std::vector<mint>{4, 5, 6}, 2), {4, 5});
  assert(fps({1, 2}, 0).precision() == 0);
  assert((a * fps(0)).precision() == 0);
  const fps unbounded{1, 2};
  assert(unbounded[100].unwrap() == 0);
  const FormalPowerSeries<long long> integer({1, 2}, 3);
  const auto square = integer * integer;
  assert(square[0] == 1 && square[1] == 4 && square[2] == 4);
}
