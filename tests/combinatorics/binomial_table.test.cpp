#include "combinatorics/binomial_table.hpp"

#include <cassert>
#include <vector>

int main() {
  const BinomialTable empty(0);
  assert(empty.choose(0, 0) == 1);

  const BinomialTable square(5);
  assert(square.choose(0, 0) == 1);
  assert(square.choose(5, 0) == 1);
  assert(square.choose(5, 5) == 1);
  assert(square.choose(5, 2) == 10);
  assert(square.choose(5, 3) == 10);
  assert(square.choose(2, 5) == 0);

  const BinomialTable narrow(1000, 2);
  assert(narrow.choose(0, 2) == 0);
  assert(narrow.choose(1000, 0) == 1);
  assert(narrow.choose(1000, 1) == 1000);
  assert(narrow.choose(1000, 2) == 499500);

  const BinomialTable wide(3, 5);
  assert(wide.choose(3, 2) == 3);
  assert(wide.choose(3, 3) == 1);
  assert(wide.choose(3, 4) == 0);
  assert(wide.choose(3, 5) == 0);

  BinomialTable zero_column(1000, 0);
  assert(zero_column.choose(0, 0) == 1);
  assert(zero_column.choose(1000, 0) == 1);
  std::vector<int> no_parts;
  std::vector<int> zeros{0, 0, 0};
  assert(zero_column.multinomial(no_parts) == 1);
  assert(zero_column.multinomial(zeros) == 1);

  const BinomialTable large_exact(66);
  assert(large_exact.choose(66, 33) == 7219428434016265740LL);

  BinomialTable exact(20);
  std::vector<int> single{20};
  std::vector<int> parts{2, 1, 2};
  std::vector<int> reordered{1, 2, 2};
  std::vector<int> with_zeros{0, 2, 0, 1, 2, 0};
  assert(exact.multinomial(single) == 1);
  assert(exact.multinomial(parts) == 30);
  assert(exact.multinomial(reordered) == 30);
  assert(exact.multinomial(with_zeros) == 30);

  BinomialTable narrow_multinomial(9, 3);
  std::vector<int> thirds{3, 3, 3};
  assert(narrow_multinomial.multinomial(thirds) == 1680);

  BinomialTable factorial(20, 1);
  std::vector<int> ones(20, 1);
  assert(factorial.multinomial(ones) == 2432902008176640000LL);

  for (int mod: {1, 2, 6, 9, 998244353, 2147483647}) {
    BinomialTable modular(20, 20, mod);
    assert(modular.choose(0, 0) == 1 % mod);
    assert(modular.choose(20, 0) == 1 % mod);
    assert(modular.choose(20, 20) == 1 % mod);
    assert(modular.choose(4, 2) == 6 % mod);
    assert(modular.choose(20, 10) == 184756 % mod);
    assert(modular.choose(3, 20) == 0);
    assert(modular.multinomial(no_parts) == 1 % mod);
    assert(modular.multinomial(zeros) == 1 % mod);
    assert(modular.multinomial(single) == 1 % mod);
    assert(modular.multinomial(parts) == 30 % mod);
    assert(modular.multinomial(with_zeros) == 30 % mod);
    assert(modular.multinomial(ones) == 2432902008176640000LL % mod);

    const BinomialTable modular_narrow(1000, 2, mod);
    assert(modular_narrow.choose(1000, 2) == 499500 % mod);
    const BinomialTable modular_wide(3, 5, mod);
    assert(modular_wide.choose(3, 3) == 1 % mod);
    assert(modular_wide.choose(3, 5) == 0);
    BinomialTable modular_zero(0, 0, mod);
    assert(modular_zero.choose(0, 0) == 1 % mod);
    assert(modular_zero.multinomial(no_parts) == 1 % mod);
    assert(modular_zero.multinomial(zeros) == 1 % mod);
    BinomialTable modular_multinomial(9, 3, mod);
    assert(modular_multinomial.multinomial(thirds) == 1680 % mod);
  }

  const BinomialTable large_mod(40, 20, 4294967296LL);
  assert(large_mod.choose(40, 20) == 407575348);

  std::vector<int> large_parts{20, 20, 20};
  BinomialTable prime_mod(60, 20, 998244353);
  assert(prime_mod.multinomial(large_parts) == 526190592);
  BinomialTable max_int_mod(60, 20, 2147483647);
  assert(max_int_mod.multinomial(large_parts) == 1460889818);
}
