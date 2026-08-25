#pragma once

#include <cassert>
#include <ranges>
#include <vector>

#include "graph/weighted_edge.hpp"
#include "template/io.hpp"

template <typename T>
class WeightedGraph {
 public:
  using edge_type = WeightedEdge<T>;

  explicit WeightedGraph(int n): n{n}, m{0}, G(n) {}
  explicit WeightedGraph(int n, int m): n{n}, m{0}, G(n) { E.reserve(2 * m); }

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

  int add_edge(int u, int v, const T& cost) {
    assert(u >= 0);
    assert(u < n);
    assert(v >= 0);
    assert(v < n);
    G[u].emplace_back(2 * m);
    E.emplace_back(u, v, cost, m);
    G[v].emplace_back(2 * m + 1);
    E.emplace_back(v, u, cost, m);
    return m++;
  }

 private:
  int n, m;
  std::vector<std::vector<int>> G;
  std::vector<edge_type> E;
};

template <typename T>
struct Input<WeightedGraph<T>> {
  static WeightedGraph<T> read(int n, int m, int offset = 0) {
    WeightedGraph<T> G{n, m};
    for (int i = 0; i < m; ++i) {
      auto u = Input<int>::read() - offset;
      auto v = Input<int>::read() - offset;
      auto cost = Input<T>::read();
      G.add_edge(u, v, cost);
    }
    return G;
  }
};
