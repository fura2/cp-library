#pragma once

#include <cassert>
#include <concepts>
#include <utility>
#include <vector>

#include "algebra/monoid.hpp"
#include "data_structure/union_find.hpp"

template <CommutativeMonoid M>
class AggregateUnionFind: UnionFind {
 public:
  using UnionFind::find;
  using UnionFind::group;
  using UnionFind::groups;
  using UnionFind::num_groups;
  using UnionFind::same;
  using UnionFind::size;

  explicit AggregateUnionFind(int n): UnionFind(n), a(n, M::identity()) {}

  template <typename T = M>
    requires std::constructible_from<M, const T&>
  explicit AggregateUnionFind(const std::vector<T>& a): UnionFind(a.size()) {
    this->a.reserve(a.size());
    for (const auto& x: a) this->a.emplace_back(x);
  }

  void unite(int u, int v) {
    assert(0 <= u && u < size());
    assert(0 <= v && v < size());
    u = find(u);
    v = find(v);
    if (u != v) {
      UnionFind::unite(u, v);
      if (find(u) == v) std::swap(u, v);
      a[u] = a[u] * a[v];
    }
  }

  const M& get(int u) const {
    assert(0 <= u && u < size());
    return a[find(u)];
  }

  template <typename T = M>
    requires std::constructible_from<M, const T&>
  void set(int u, const T& x) {
    assert(0 <= u && u < size());
    a[find(u)] = M(x);
  }

 private:
  std::vector<M> a;
};
