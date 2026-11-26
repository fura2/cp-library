#pragma once

#include <cassert>

#include "graph/weighted_graph.hpp"
#include "template/io.hpp"

template <typename T>
class WeightedTree {
 public:
  using edge_type = WeightedGraph<T>::edge_type;
  static constexpr bool directed = WeightedGraph<T>::directed;

  explicit WeightedTree(int n) {
    assert(n >= 1);
    G = WeightedGraph<T>{n, n - 1};
  }

  int num_vertices() const { return G.num_vertices(); }
  int num_edges() const { return G.num_edges(); }

  int degree(int u) const { return G.degree(u); }

  auto operator[](int u) const { return G[u]; }

  const edge_type& edge(int i) const { return G.edge(i); }

  int add_edge(int u, int v, const T& cost) { return G.add_edge(u, v, cost); }

 private:
  WeightedGraph<T> G;
};

template <typename T>
struct Input<WeightedTree<T>> {
  static WeightedTree<T> read(int n, int offset = 0) {
    WeightedTree<T> U{n};
    for (int i = 0; i < n - 1; ++i) {
      auto u = Input<int>::read() - offset;
      auto v = Input<int>::read() - offset;
      auto cost = Input<T>::read();
      U.add_edge(u, v, cost);
    }
    return U;
  }
};
