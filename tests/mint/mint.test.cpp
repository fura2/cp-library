#include "mint/mint.hpp"

#include <cassert>
#include <vector>

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

  std::vector<int> no_parts;
  std::vector<int> zeros{0, 0, 0};
  std::vector<int> single{20};
  std::vector<int> two_parts{4, 3};
  const std::vector<int> parts{2, 1, 3};
  std::vector<int> reordered{3, 2, 1};
  std::vector<int> with_zeros{0, 2, 0, 1, 3, 0};
  std::vector<int> repeated{3, 3, 3};
  assert(multinomial(no_parts).unwrap() == 1);
  assert(multinomial(zeros).unwrap() == 1);
  assert(multinomial(single).unwrap() == 1);
  assert(multinomial(two_parts).unwrap() == 35);
  assert(multinomial(parts).unwrap() == 60);
  assert(multinomial({2, 1, 3}).unwrap() == 60);
  assert(multinomial(reordered).unwrap() == 60);
  assert(multinomial(with_zeros).unwrap() == 60);
  assert(multinomial(repeated).unwrap() == 1680);

  std::vector<int> ones(20, 1);
  assert(multinomial(ones).unwrap() == 2432902008176640000LL % mod);
  std::vector<int> large_parts{20, 20, 20};
  assert(multinomial(large_parts).unwrap() == 526190592);

  assert(pretty(mint{42}) == "42");
  TestIO io{"-1"};
  const auto read = input<mint>();
  assert(read.unwrap() == mod - 1);
  output(read);
  assert(io.out.str() == "998244352\n");
}
