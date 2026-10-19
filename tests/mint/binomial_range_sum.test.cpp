#include "mint/binomial_range_sum.hpp"

#include <cassert>
#include <concepts>

#include "mint/mint.hpp"

template <typename T, bool Accepted>
void check_constraints() {
  static_assert(requires { typename BinomialRangeSum<T>; } == Accepted);
}

static_assert(!std::default_initializable<BinomialRangeSum<mint>>);

int main() {
  check_constraints<mint, true>();
  check_constraints<ModInt<3>, true>();
  check_constraints<ModInt<2>, false>();
  check_constraints<ModInt<1>, false>();
  check_constraints<int, false>();
  check_constraints<const mint, false>();
  check_constraints<mint&, false>();

  BinomialRangeSum<mint> range(3, 1, 3);
  assert(range.sum(3, 1, 3).unwrap() == 6);
  // Moving the left endpoint right removes terms; moving it left adds them.
  assert(range.sum(3, 2, 3).unwrap() == 3);
  assert(range.sum(3, 0, 3).unwrap() == 7);
  assert(range.sum(3, 0, 4).unwrap() == 8);
  assert(range.sum(3, 0, 2).unwrap() == 4);
  assert(range.sum(3, 1, 3).unwrap() == 6);
  assert(range.sum(5, 1, 3).unwrap() == 15);
  assert(range.sum(2, 1, 3).unwrap() == 3);

  // Move all coordinates, including disjoint intervals and terms beyond n.
  assert(range.sum(4, 5, 8).unwrap() == 0);
  assert(range.sum(4, 3, 8).unwrap() == 5);
  assert(range.sum(2, 0, 3).unwrap() == 4);
  assert(range.sum(6, 0, 7).unwrap() == 64);
  assert(range.sum(6, 0, 7).unwrap() == 64);
  assert(range.sum(0, 0, 1).unwrap() == 1);
  assert(range.sum(0, 0, 0).unwrap() == 0);

  BinomialRangeSum<mint> empty(4, 2, 2);
  assert(empty.sum(4, 2, 2).unwrap() == 0);
  assert(empty.sum(4, 2, 5).unwrap() == 11);
  assert(empty.sum(4, 3, 3).unwrap() == 0);

  BinomialRangeSum<mint> outside(3, 5, 7);
  assert(outside.sum(3, 5, 7).unwrap() == 0);
  assert(outside.sum(3, 3, 7).unwrap() == 1);

  BinomialRangeSum<mint> zero(0, 0, 1);
  assert(zero.sum(0, 0, 1).unwrap() == 1);
  assert(zero.sum(0, 1, 1).unwrap() == 0);

  // The smallest supported prime exercises modular wraparound and halving.
  BinomialRangeSum<ModInt<3>> small(2, 0, 3);
  assert(small.sum(2, 0, 3).unwrap() == 1);
  assert(small.sum(1, 0, 2).unwrap() == 2);
  assert(small.sum(2, 1, 3).unwrap() == 0);
  assert(small.sum(0, 0, 1).unwrap() == 1);
}
