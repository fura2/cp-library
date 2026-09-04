#pragma once

#include <cassert>
#include <iterator>
#include <utility>
#include <vector>

class UnionFind {
 public:
  explicit UnionFind(int n): n{n}, p(n, -1) {}

  int find(int u) const {
    assert(0 <= u && u < std::ssize(p));
    return p[u] < 0 ? u : p[u] = find(p[u]);
  }

  void unite(int u, int v) {
    assert(0 <= u && u < std::ssize(p));
    assert(0 <= v && v < std::ssize(p));
    u = find(u);
    v = find(v);
    if (u != v) {
      if (p[v] < p[u]) std::swap(u, v);
      p[u] += p[v];
      p[v] = u;
      n--;
    }
  }

  bool same(int u, int v) const {
    assert(0 <= u && u < std::ssize(p));
    assert(0 <= v && v < std::ssize(p));
    return find(u) == find(v);
  }

  int size() const { return std::ssize(p); }

  int size(int u) const {
    assert(0 <= u && u < std::ssize(p));
    return -p[find(u)];
  }

  int num_groups() const { return n; }

  std::vector<std::vector<int>> groups() const {
    std::vector<int> id(p.size(), -1);
    std::vector<std::vector<int>> res;
    res.reserve(n);
    for (int u = 0; u < std::ssize(p); ++u) {
      int r = find(u);
      if (id[r] == -1) {
        id[r] = res.size();
        res.emplace_back();
      }
      res[id[r]].emplace_back(u);
    }
    return res;
  }

 private:
  int n;
  mutable std::vector<int> p;
};
