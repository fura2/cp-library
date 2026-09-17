#include "data_structure/union_find.hpp"

#include <algorithm>
#include <cassert>
#include <vector>

int main() {
  const UnionFind empty{0};
  assert(empty.size() == 0 && empty.num_groups() == 0 &&
         empty.groups().empty());
  UnionFind uf{5};
  assert(uf.size() == 5 && uf.num_groups() == 5);
  assert(uf.same(0, 0) && !uf.same(0, 1) && uf.size(0) == 1);
  uf.unite(0, 2);
  uf.unite(2, 4);
  uf.unite(1, 3);
  uf.unite(4, 0);
  uf.unite(2, 2);
  assert(uf.num_groups() == 2 && uf.size(4) == 3 && uf.size(3) == 2);
  assert(uf.same(0, 4) && !uf.same(0, 3));
  assert(uf.find(0) == uf.find(4));
  assert((uf.group(2) == std::vector<int>{0, 2, 4}));
  auto groups = uf.groups();
  std::ranges::sort(groups);
  assert((groups == std::vector<std::vector<int>>{{0, 2, 4}, {1, 3}}));
  uf.unite(3, 4);
  const auto& view = uf;
  assert(view.num_groups() == 1 && view.size(1) == 5 && view.same(0, 3));
}
