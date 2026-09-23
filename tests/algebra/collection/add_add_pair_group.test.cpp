#include "algebra/collection/add_add_pair_group.hpp"

#include <cassert>
#include <concepts>
#include <vector>

#include "algebra/pair_monoid.hpp"
#include "data_structure/fenwick_tree.hpp"

using P = LintAddIntAddPairGroup;
using M = PairMonoid<LintAddGroup, IntAddGroup>;

template <typename T>
concept HasRangeFold =
    requires(const FenwickTree<T>& tree) { tree.fold(0, 0); };

static_assert(std::same_as<P, PairGroup<LintAddGroup, IntAddGroup>>);
static_assert(Group<P> && !Group<M>);
static_assert(HasRangeFold<P> && !HasRangeFold<M>);

void check(const P& p, long long first, int second) {
  assert(p.first().unwrap() == first);
  assert(p.second().unwrap() == second);
}

int main() {
  const P a{3'000'000'000LL, 1}, b{4'000'000'000LL, 2};
  check(P{}, 0, 0);
  check(P::identity(), 0, 0);
  check(a * b, 7'000'000'000LL, 3);
  check(a.inverse(), -3'000'000'000LL, -1);
  check(a * a.inverse(), 0, 0);
  check(a.inverse() * a, 0, 0);
  check(a * P::identity(), 3'000'000'000LL, 1);
  check(P::identity() * a, 3'000'000'000LL, 1);

  // Store a sum and a count in each entry.
  FenwickTree<P> tree{std::vector<P>{a, b, P{-2'000'000'000LL, 1}}};
  check(tree.fold(), 5'000'000'000LL, 4);
  check(tree.fold(1, 3), 2'000'000'000LL, 3);
  check(tree.fold(1, 1), 0, 0);
  tree.set(0, P{1'000'000'000LL, 2});
  check(tree.get(0), 1'000'000'000LL, 2);
  check(tree.fold(), 3'000'000'000LL, 5);
  tree.apply(1, P{-1'000'000'000LL, 1});
  check(tree.fold(), 2'000'000'000LL, 6);
  const FenwickTree<P> empty{0};
  check(empty.fold(0, 0), 0, 0);
}
