#include "data_structure/sparse_table.hpp"

#include <cassert>
#include <vector>

#include "algebra/collection/max_monoid.hpp"
#include "algebra/collection/min_monoid.hpp"
#include "algebra/pair_monoid.hpp"
#include "algebra/semigroup_impl.hpp"

using Minimum = SemigroupImpl<int, [](int a, int b) { return a < b ? a : b; }>;
static_assert(!Monoid<Minimum>);

int main() {
  const SparseTable<IntMinMonoid> empty{std::vector<int>{}};
  assert(empty.size() == 0 && empty.fold(0, 0).unwrap() == INF);
  const SparseTable<IntMinMonoid> single{std::vector<int>{7}};
  assert(single.fold(0, 1).unwrap() == 7 && single.fold(1, 1).unwrap() == INF);
  const std::vector<int> values = {5, 2, 4, 1, 3};
  const SparseTable<IntMinMonoid> table{values};
  assert(table.size() == 5 && table.fold(0, 5).unwrap() == 1);
  assert(table.fold(0, 3).unwrap() == 2 && table.fold(2, 5).unwrap() == 1);
  assert(table.fold(4, 5).unwrap() == 3 && table.fold(2, 2).unwrap() == INF);
  const SparseTable<Minimum> semigroup{values};
  assert(semigroup.fold(0, 5).unwrap() == 1);
  assert(semigroup.fold(0, 3).unwrap() == 2);

  using Bounds = PairMonoid<IntMinMonoid, IntMaxMonoid>;
  const SparseTable<Bounds> bounds({{4, 4}, {1, 1}, {7, 7}});
  assert(bounds.size() == 3);
  assert(bounds.fold(0, 2).first.unwrap() == 1);
  assert(bounds.fold(0, 2).second.unwrap() == 4);
  assert(bounds.fold(1, 3).first.unwrap() == 1);
  assert(bounds.fold(1, 3).second.unwrap() == 7);
  const SparseTable<Bounds> empty_bounds({});
  assert(empty_bounds.size() == 0);
  assert(empty_bounds.fold(0, 0).first.unwrap() == INF);
  assert(empty_bounds.fold(0, 0).second.unwrap() == -INF);
}
