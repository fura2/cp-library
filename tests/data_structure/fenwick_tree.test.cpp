#include "data_structure/fenwick_tree.hpp"

#include <cassert>
#include <vector>

#include "algebra/collection/add_add_pair_group.hpp"
#include "algebra/collection/add_group.hpp"
#include "algebra/collection/max_monoid.hpp"

int main() {
  auto at_most = [](int limit) {
    return [limit](const IntAddGroup& x) { return x.unwrap() <= limit; };
  };
  const FenwickTree<IntAddGroup> empty{0};
  assert(empty.size() == 0 && empty.fold().unwrap() == 0);
  assert(empty.fold(0, 0).unwrap() == 0);
  assert(empty.max_right(at_most(0)) == 0);
  assert(empty.max_right(0, at_most(0)) == 0);
  assert(empty.min_left(0, at_most(0)) == 0);
  FenwickTree<IntAddGroup> single{1};
  single.apply(0, 7);
  assert(single.get(0).unwrap() == 7);
  single.set(0, 2);
  assert(single.fold().unwrap() == 2);

  FenwickTree<IntAddGroup> tree{std::vector<int>{2, 1, 3, 0, 4}};
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
  FenwickTree<IntMaxMonoid> maximum{std::vector<int>{-2, 5, 1}};
  assert(maximum.fold(0).unwrap() == -INF);
  assert(maximum.fold(2).unwrap() == 5);
  maximum.apply(2, 9);
  assert(maximum.fold().unwrap() == 9);
  assert(maximum.max_right(
             [](const IntMaxMonoid& x) { return x.unwrap() <= 5; }) == 2);
  assert(pretty(maximum) == "[3 element(s)]");

  FenwickTree<LintAddIntAddPairGroup> pairs({{1, 10}, {2, 20}, {3, 30}});
  assert(pairs.size() == 3);
  assert(pairs.fold().first.unwrap() == 6);
  assert(pairs.fold().second.unwrap() == 60);
  pairs.apply(1, {4, -5});
  assert(pairs.get(1).first.unwrap() == 6);
  assert(pairs.get(1).second.unwrap() == 15);
  pairs.set(0, {-1, 7});
  assert(pairs.fold().first.unwrap() == 8);
  assert(pairs.fold().second.unwrap() == 52);
}
