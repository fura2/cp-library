#include "data_structure/aggregate_union_find.hpp"

#include <cassert>
#include <vector>

#include "algebra/collection/add_monoid.hpp"

int main() {
  const AggregateUnionFind<AddMonoid<int>> empty{0};
  assert(empty.size() == 0 && empty.num_groups() == 0);
  const AggregateUnionFind<AddMonoid<int>> single{1};
  assert(single.get(0).unwrap() == 0);

  AggregateUnionFind<AddMonoid<int>> uf{std::vector<long long>{2, 3, 5, 7}};
  uf.unite(0, 1);
  uf.unite(2, 1);  // The larger component is the second argument.
  assert(uf.get(0).unwrap() == 10 && uf.get(2).unwrap() == 10);

  uf.set(2, 11LL);
  assert(uf.get(1).unwrap() == 11 && uf.get(3).unwrap() == 7);
  uf.unite(0, 2);
  assert(uf.get(1).unwrap() == 11 && uf.num_groups() == 2);
  uf.unite(3, 2);
  assert(uf.get(0).unwrap() == 18);
}
