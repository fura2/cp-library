#pragma once

#include <cassert>
#include <ranges>
#include <vector>

#include "graph/edge.hpp"
#include "template/io.hpp"

class Digraph {
 public:
  using edge_type = Edge;

  explicit Digraph(int n): n{n}, m{0}, G(n) {}
  explicit Digraph(int n, int m): n{n}, m{0}, G(n) { E.reserve(m); }

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
    return E[i];
  }

  int add_edge(int from, int to) {
    assert(from >= 0);
    assert(from < n);
    assert(to >= 0);
    assert(to < n);
    G[from].emplace_back(m);
    E.emplace_back(from, to, m);
    return m++;
  }

 private:
  int n, m;
  std::vector<std::vector<int>> G;
  std::vector<edge_type> E;
};

template <>
struct Input<Digraph> {
  static Digraph read(int n, int m, int offset = 0) {
    Digraph G{n, m};
    for (int i = 0; i < m; ++i) {
      auto from = Input<int>::read() - offset;
      auto to = Input<int>::read() - offset;
      G.add_edge(from, to);
    }
    return G;
  }
};
