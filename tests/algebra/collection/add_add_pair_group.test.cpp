#include "algebra/collection/add_add_pair_group.hpp"

#include <cassert>
#include <concepts>
#include <vector>

#include "algebra/pair_monoid.hpp"
#include "data_structure/fenwick_tree.hpp"

using P = LintAddIntAddPairGroup;
using Q = LintAddLintAddPairGroup;
using M = PairMonoid<LintAddGroup, IntAddGroup>;

template <typename T>
concept HasRangeFold =
    requires(const FenwickTree<T>& tree) { tree.fold(0, 0); };

static_assert(std::same_as<P, PairGroup<LintAddGroup, IntAddGroup>>);
static_assert(std::same_as<Q, PairGroup<LintAddGroup, LintAddGroup>>);
static_assert(Group<P> && Group<Q> && !Group<M>);
static_assert(HasRangeFold<P> && HasRangeFold<Q> && !HasRangeFold<M>);

template <Group G>
void check(const G& p, long long first, long long second) {
  assert(p.first.unwrap() == first);
  assert(p.second.unwrap() == second);
}

void check_lint_int() {
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

void check_lint_lint() {
  // Both components exceed the 32-bit signed integer range.
  const Q a{3'000'000'000LL, -4'000'000'000LL};
  const Q b{5'000'000'000LL, 6'000'000'000LL};
  check(Q{}, 0, 0);
  check(Q::identity(), 0, 0);
  check(a * b, 8'000'000'000LL, 2'000'000'000LL);
  check(a.inverse(), -3'000'000'000LL, 4'000'000'000LL);
  check(a * a.inverse(), 0, 0);
  check(a.inverse() * a, 0, 0);

  FenwickTree<Q> tree{std::vector<Q>{a, b}};
  check(tree.fold(), 8'000'000'000LL, 2'000'000'000LL);
  check(tree.fold(1, 2), 5'000'000'000LL, 6'000'000'000LL);
  check(tree.fold(1, 1), 0, 0);
  tree.set(0, Q{-7'000'000'000LL, 9'000'000'000LL});
  check(tree.get(0), -7'000'000'000LL, 9'000'000'000LL);
  check(tree.fold(), -2'000'000'000LL, 15'000'000'000LL);
  tree.apply(1, Q{6'000'000'000LL, -3'000'000'000LL});
  check(tree.fold(), 4'000'000'000LL, 12'000'000'000LL);
}

int main() {
  check_lint_int();
  check_lint_lint();
}
