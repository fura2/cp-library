#include "mint/mod_int.hpp"

#include <cassert>
#include <concepts>
#include <limits>
#include <vector>

#include "mint/mint.hpp"
#include "tests/support/io.hpp"

static_assert(std::same_as<mint, ModInt<998244353>>);
static_assert(mint::mod() == 998244353);
static_assert(ModInt<7>::mod() == 7);

struct HasInverse {
  HasInverse inverse() const;
};

struct DerivedModInt: mint {};

template <typename T, bool Accepted>
void check_constraints() {
  static_assert(is_mod_int<T> == Accepted);
  static_assert(requires(T x) { pow<T>(x, 0); } == Accepted);
  static_assert(requires { inverse<T>(1); } == Accepted);
  static_assert(requires { factorial<T>(0); } == Accepted);
  static_assert(requires { factorial_inverse<T>(0); } == Accepted);
  static_assert(requires { choose<T>(0, 0); } == Accepted);
  static_assert(requires { multichoose<T>(0, 0); } == Accepted);
  static_assert(requires { multinomial<T>({}); } == Accepted);
}

int main() {
  check_constraints<mint, true>();
  check_constraints<ModInt<7>, true>();
  check_constraints<int, false>();
  check_constraints<HasInverse, false>();
  check_constraints<DerivedModInt, false>();
  check_constraints<const mint, false>();
  check_constraints<mint&, false>();

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

  // Extend the inverse cache in steps, then revisit earlier entries.
  for (int n: {1, 2, 8, 3, 12, 1}) {
    assert((mint{n} * inverse<mint>(n)).unwrap() == 1);
  }
  assert(inverse<ModInt<2>>(1).unwrap() == 1);
  assert(factorial_inverse<ModInt<2>>(0).unwrap() == 1);
  assert(factorial_inverse<ModInt<2>>(1).unwrap() == 1);

  assert(factorial<mint>(0).unwrap() == 1);
  assert(factorial<mint>(5).unwrap() == 120);
  assert(factorial<mint>(3).unwrap() == 6);
  assert(factorial_inverse<mint>(0).unwrap() == 1);
  assert((factorial<mint>(8) * factorial_inverse<mint>(8)).unwrap() == 1);
  assert((factorial<mint>(3) * factorial_inverse<mint>(3)).unwrap() == 1);
  assert((factorial<mint>(10) * factorial_inverse<mint>(10)).unwrap() == 1);
  assert(factorial_inverse<mint>(0).unwrap() == 1);
  for (int type: {0, 1}) {
    assert(choose<mint>(5, 2, type).unwrap() == 10);
    assert(choose<mint>(5, 0, type).unwrap() == 1);
    assert(choose<mint>(5, 5, type).unwrap() == 1);
    assert(choose<mint>(5, -1, type).unwrap() == 0);
    assert(choose<mint>(5, 6, type).unwrap() == 0);
    assert(multichoose<mint>(3, 2, type).unwrap() == 6);
    assert(multichoose<mint>(0, 0, type).unwrap() == 1);
    assert(multichoose<mint>(0, 2, type).unwrap() == 0);
    assert(multichoose<mint>(3, 0, type).unwrap() == 1);
  }

  std::vector<int> no_parts;
  std::vector<int> zeros{0, 0, 0};
  std::vector<int> single{20};
  std::vector<int> two_parts{4, 3};
  const std::vector<int> parts{2, 1, 3};
  std::vector<int> reordered{3, 2, 1};
  std::vector<int> with_zeros{0, 2, 0, 1, 3, 0};
  std::vector<int> repeated{3, 3, 3};
  assert(multinomial<mint>(no_parts).unwrap() == 1);
  assert(multinomial<mint>(zeros).unwrap() == 1);
  assert(multinomial<mint>(single).unwrap() == 1);
  assert(multinomial<mint>(two_parts).unwrap() == 35);
  assert(multinomial<mint>(parts).unwrap() == 60);
  assert(multinomial<mint>({2, 1, 3}).unwrap() == 60);
  assert(multinomial<mint>(reordered).unwrap() == 60);
  assert(multinomial<mint>(with_zeros).unwrap() == 60);
  assert(multinomial<mint>(repeated).unwrap() == 1680);

  std::vector<int> ones(20, 1);
  assert(multinomial<mint>(ones).unwrap() == 2432902008176640000LL % mod);
  std::vector<int> large_parts{20, 20, 20};
  assert(multinomial<mint>(large_parts).unwrap() == 526190592);

  // All caches must be independent for each modulus.
  using Mod7 = ModInt<7>;
  using Mod11 = ModInt<11>;
  assert(inverse<Mod7>(3).unwrap() == 5);
  assert(inverse<Mod11>(3).unwrap() == 4);
  assert(inverse<Mod7>(6).unwrap() == 6);
  assert(inverse<Mod7>(3).unwrap() == 5);
  assert(factorial<Mod7>(5).unwrap() == 1);
  assert(factorial_inverse<Mod7>(5).unwrap() == 1);
  // An inverse factorial can be requested before the factorial itself.
  assert(factorial_inverse<Mod11>(5).unwrap() == 10);
  assert(factorial<Mod11>(5).unwrap() == 10);
  assert(factorial<Mod7>(3).unwrap() == 6);
  assert(factorial_inverse<Mod7>(3).unwrap() == 6);
  assert(factorial_inverse<Mod7>(6).unwrap() == 6);
  assert(factorial<mint>(5).unwrap() == 120);
  for (int type: {0, 1}) {
    assert(choose<Mod7>(5, 2, type).unwrap() == 3);
  }
  assert(choose<Mod7>(9, 2, 1).unwrap() == 1);
  assert(multinomial<Mod7>({2, 1, 3}).unwrap() == 4);

  // Inverses also exist for units modulo a composite number.
  using Mod15 = ModInt<15>;
  assert(Mod15{7}.inverse().unwrap() == 13);
  assert((Mod15{14} / Mod15{7}).unwrap() == 2);

  constexpr int max_mod = std::numeric_limits<int>::max() / 2;
  using MaxMod = ModInt<max_mod>;
  const MaxMod last{max_mod - 1};
  assert((last + last).unwrap() == max_mod - 2);
  assert((last * last).unwrap() == 1);
  assert(last.inverse().unwrap() == max_mod - 1);

  using Mod1 = ModInt<1>;
  assert(Mod1{-1}.unwrap() == 0);
  assert(pow(Mod1{0}, 0).unwrap() == 0);
  assert(factorial<Mod1>(0).unwrap() == 0);

  assert(pretty(mint{42}) == "42");
  TestIO io{"-1 -8"};
  const auto read = input<mint>();
  assert(read.unwrap() == mod - 1);
  output(read);
  const auto read_mod7 = input<Mod7>();
  assert(read_mod7.unwrap() == 6);
  output(read_mod7);
  assert(io.out.str() == "998244352\n6\n");
}
