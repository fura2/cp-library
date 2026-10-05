#include "data_structure/fenwick_tree.hpp"

#include <cassert>
#include <vector>

#include "algebra/collection/add_group.hpp"
#include "algebra/collection/max_monoid.hpp"
#include "algebra/pair_group.hpp"
#include "algebra/pair_monoid.hpp"

using SumCount = PairGroup<AddGroup<long long>, AddGroup<int>>;
using MonoidPair = PairMonoid<AddGroup<long long>, AddGroup<int>>;

template <typename T>
concept HasRangeFold =
    requires(const FenwickTree<T>& tree) { tree.fold(0, 0); };

static_assert(HasRangeFold<SumCount> && !HasRangeFold<MonoidPair>);

void check(const SumCount& value, long long sum, int count) {
  assert(value.first.unwrap() == sum && value.second.unwrap() == count);
}

int main() {
  auto at_most = [](int limit) {
    return [limit](const AddGroup<int>& x) { return x.unwrap() <= limit; };
  };
  const FenwickTree<AddGroup<int>> empty{0};
  assert(empty.size() == 0 && empty.fold().unwrap() == 0);
  assert(empty.fold(0, 0).unwrap() == 0);
  assert(empty.max_right(at_most(0)) == 0);
  assert(empty.max_right(0, at_most(0)) == 0);
  assert(empty.min_left(0, at_most(0)) == 0);
  FenwickTree<AddGroup<int>> single{1};
  single.apply(0, 7);
  assert(single.get(0).unwrap() == 7);
  single.set(0, 2);
  assert(single.fold().unwrap() == 2);

  FenwickTree<AddGroup<int>> tree{std::vector<int>{2, 1, 3, 0, 4}};
  assert(tree.size() == 5 && tree.fold().unwrap() == 10);
  assert(tree.fold(3).unwrap() == 6 && tree.fold(1, 4).unwrap() == 4);
  assert(tree.fold(2, 2).unwrap() == 0);
  assert(tree.max_right(at_most(3)) == 2);
  assert(tree.max_right(at_most(0)) == 0);
  assert(tree.max_right(at_most(100)) == 5);
  assert(tree.max_right(1, at_most(4)) == 4);
  assert(tree.max_right(3, at_most(0)) == 4);
  assert(tree.max_right(5, at_most(0)) == 5);
  assert(tree.min_left(5, at_most(4)) == 3);
  assert(tree.min_left(4, at_most(4)) == 1);
  assert(tree.min_left(5, at_most(0)) == 5);
  assert(tree.min_left(5, at_most(100)) == 0);
  assert(tree.min_left(0, at_most(0)) == 0);
  tree.apply(2, -1);
  tree.set(4, 8);
  assert(tree.get(2).unwrap() == 2 && tree.get(4).unwrap() == 8);
  assert(tree.fold().unwrap() == 13);
  assert(pretty(tree) == "[5 element(s)]");

  // Prefix folds and updates also work for a monoid without inverses.
  FenwickTree<MaxMonoid<int>> maximum{std::vector<int>{-2, 5, 1}};
  assert(maximum.fold(0).unwrap() == -INF);
  assert(maximum.fold(2).unwrap() == 5);
  maximum.apply(2, 9);
  assert(maximum.fold().unwrap() == 9);
  assert(maximum.max_right(
             [](const MaxMonoid<int>& x) { return x.unwrap() <= 5; }) == 2);
  assert(pretty(maximum) == "[3 element(s)]");

  // Sum and count use different types; sums exceed the 32-bit integer range.
  FenwickTree<SumCount> pairs(
      {{3'000'000'000LL, 1}, {4'000'000'000LL, 2}, {-2'000'000'000LL, 1}});
  assert(pairs.size() == 3);
  check(pairs.fold(), 5'000'000'000LL, 4);
  check(pairs.fold(1, 3), 2'000'000'000LL, 3);
  check(pairs.fold(1, 1), 0, 0);
  pairs.set(0, {1'000'000'000LL, 2});
  check(pairs.get(0), 1'000'000'000LL, 2);
  check(pairs.fold(), 3'000'000'000LL, 5);
  pairs.apply(1, {-1'000'000'000LL, 1});
  check(pairs.get(1), 3'000'000'000LL, 3);
  check(pairs.fold(), 2'000'000'000LL, 6);
}
