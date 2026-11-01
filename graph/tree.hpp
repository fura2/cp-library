#pragma once

#include <cassert>
#include <utility>
#include <vector>

#include "graph/graph.hpp"
#include "template/io.hpp"

class Tree {
 public:
  using edge_type = Graph::edge_type;
  static constexpr bool directed = Graph::directed;

  explicit Tree(int n) {
    assert(n >= 1);
    G = Graph{n, n - 1};
  }

  int num_vertices() const { return G.num_vertices(); }
  int num_edges() const { return G.num_edges(); }

  int degree(int u) const { return G.degree(u); }

  auto operator[](int u) const { return G[u]; }

  const edge_type& edge(int i) const { return G.edge(i); }

  int add_edge(int u, int v) { return G.add_edge(u, v); }

  std::pair<std::vector<int>, int> centers() const {
    int n = num_vertices();

    int far = -1, d_far = -1;
    std::vector<int> prev(n);
    auto dfs = [&](auto&& dfs, int u, int p, int d) -> void {
      if (d_far < d) {
        d_far = d;
        far = u;
      }
      for (const auto& e: (*this)[u]) {
        if (e.to == p) continue;
        prev[e.to] = u;
        dfs(dfs, e.to, u, d + 1);
      }
    };
    dfs(dfs, 0, -1, 0);
    int v0 = far;

    far = d_far = -1;
    dfs(dfs, v0, -1, 0);
    int v1 = far;

    std::vector<int> P;
    for (int u = v1; u != v0; u = prev[u]) {
      P.emplace_back(u);
    }
    P.emplace_back(v0);

    int m = P.size();
    if (m % 2 == 1) {
      return {{P[m / 2]}, m - 1};
    }
    else {
      return {{P[m / 2 - 1], P[m / 2]}, m - 1};
    }
  }

 private:
  Graph G;
};

template <>
struct Input<Tree> {
  static Tree read(int n, int offset = 0) {
    Tree T{n};
    for (int i = 0; i < n - 1; ++i) {
      auto u = Input<int>::read() - offset;
      auto v = Input<int>::read() - offset;
      T.add_edge(u, v);
    }
    return T;
  }
};
