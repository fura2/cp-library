#include "mint/mint.hpp"

#include <cassert>

#include "tests/support/io.hpp"

int main() {
  constexpr int mod = 998244353;
  assert(mint{}.unwrap() == 0);
  assert(mint{-1}.unwrap() == mod - 1);
  assert(mint{2LL * mod + 3}.unwrap() == 3);
  assert((mint{mod - 1} + mint{2}).unwrap() == 1);
  assert((mint{1} - mint{2}).unwrap() == mod - 1);
  assert((mint{mod - 1} * mint{mod - 1}).unwrap() == 1);
  assert((mint{12} / mint{3}).unwrap() == 4);
  assert((-mint{0}).unwrap() == 0);
  assert((-mint{3}).unwrap() == mod - 3);
  assert((mint{7} * mint{7}.inverse()).unwrap() == 1);
  assert((2 + mint{3}).unwrap() == 5);
  assert((2 - mint{3}).unwrap() == mod - 1);
  assert((2 * mint{3}).unwrap() == 6);
  assert((12 / mint{3}).unwrap() == 4);
  mint x = 4;
  x += 5;
  x -= 2;
  x *= 6;
  x /= 7;
  assert(x.unwrap() == 6);
  assert(pow(mint{2}, 10).unwrap() == 1024);
  assert(pow(mint{0}, 0).unwrap() == 1);
  assert(pow(mint{2}, mod - 1).unwrap() == 1);

  assert(factorial(0).unwrap() == 1);
  assert(factorial(5).unwrap() == 120);
  assert(factorial(3).unwrap() == 6);
  assert(factorial_inverse(0).unwrap() == 1);
  assert((factorial(8) * factorial_inverse(8)).unwrap() == 1);
  assert((factorial(3) * factorial_inverse(3)).unwrap() == 1);
  assert((factorial(10) * factorial_inverse(10)).unwrap() == 1);
  for (int type: {0, 1}) {
    assert(choose(5, 2, type).unwrap() == 10);
    assert(choose(5, 0, type).unwrap() == 1);
    assert(choose(5, 5, type).unwrap() == 1);
    assert(choose(5, -1, type).unwrap() == 0);
    assert(choose(5, 6, type).unwrap() == 0);
    assert(multichoose(3, 2, type).unwrap() == 6);
    assert(multichoose(0, 0, type).unwrap() == 1);
    assert(multichoose(0, 2, type).unwrap() == 0);
    assert(multichoose(3, 0, type).unwrap() == 1);
  }
  assert(pretty(mint{42}) == "42");
  TestIO io{"-1"};
  const auto read = input<mint>();
  assert(read.unwrap() == mod - 1);
  output(read);
  assert(io.out.str() == "998244352\n");
}
