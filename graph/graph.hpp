#pragma once

#include <cassert>
#include <ranges>
#include <vector>

#include "graph/edge.hpp"

class Graph {
 public:
  using edge_type = Edge;

  explicit Graph(int n): n{n}, m{0}, G(n) {}
  explicit Graph(int n, int m): n{n}, m{0}, G(n) { E.reserve(2 * m); }

  int num_vertices() const { return n; }
  int num_edges() const { return m; }

  auto operator[](int u) const {
    assert(u >= 0);
    assert(u < n);
    return std::views::transform(
        G[u], [this](int i) -> const edge_type& { return E[i]; });
  }

  const edge_type& edge(int i) const {
    assert(i >= 0);
    assert(i < m);
    return E[2 * i];
  }

  int add_edge(int u, int v) {
    assert(u >= 0);
    assert(u < n);
    assert(v >= 0);
    assert(v < n);
    G[u].emplace_back(2 * m);
    E.emplace_back(u, v, m);
    G[v].emplace_back(2 * m + 1);
    E.emplace_back(v, u, m);
    return m++;
  }

 private:
  int n, m;
  std::vector<std::vector<int>> G;
  std::vector<edge_type> E;
};
