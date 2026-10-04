#pragma once

#include <algorithm>
#include <cassert>
#include <iterator>
#include <unordered_map>
#include <vector>

#include "graph/digraph.hpp"
#include "graph/incidence_graph.hpp"

class StronglyConnectedComponents {
 public:
  template <typename GraphT>
    requires IncidenceGraph<GraphT> && Directed<GraphT>
  StronglyConnectedComponents(const GraphT& G): n{G.num_vertices()} {
    int m = G.num_edges();

    Digraph G_rev(n, m);
    for (int u = 0; u < n; ++u) {
      for (const auto& e: G[u]) {
        G_rev.add_edge(e.to, u);
      }
    }

    int k;
    std::vector<int> top(n);

    auto dfs1 = [&](auto&& dfs1, int u) -> void {
      id[u] = 0;
      for (const auto& e: G[u]) {
        if (id[e.to] == -1) {
          dfs1(dfs1, e.to);
        }
      }
      top[k] = u;
      ++k;
    };
    auto dfs2 = [&](auto&& dfs2, int u) -> void {
      id[u] = k;
      for (const auto& e: G_rev[u]) {
        if (id[e.to] == -1) {
          dfs2(dfs2, e.to);
        }
      }
    };

    k = 0;
    id.assign(n, -1);
    for (int u = 0; u < n; ++u) {
      if (id[u] == -1) {
        dfs1(dfs1, u);
      }
    }

    std::ranges::reverse(top);

    k = 0;
    id.assign(n, -1);
    for (int u: top) {
      if (id[u] == -1) {
        dfs2(dfs2, u);
        ++k;
      }
    }

    scc.assign(k, {});
    D = Digraph(k);
    for (int u = 0; u < n; ++u) {
      scc[id[u]].emplace_back(u);
      for (const auto& e: G[u]) {
        long long h = hash(id[u], id[e.to]);
        auto [it, inserted] = E.try_emplace(h);
        if (id[u] != id[e.to] && inserted) {
          D.add_edge(id[u], id[e.to]);
        }
        it->second.emplace_back(e.id);
      }
    }
  }

  int size() const { return scc.size(); }

  int component_id(int u) const {
    assert(0 <= u && u < n);
    return id[u];
  }

  const std::vector<int>& component(int i) const {
    assert(0 <= i && i < std::ssize(scc));
    return scc[i];
  }

  const std::vector<std::vector<int>>& components() const { return scc; }

  const std::vector<int>& edge_ids(int i, int j) const {
    assert(0 <= i && i < std::ssize(scc));
    assert(0 <= j && j < std::ssize(scc));
    return E[hash(i, j)];
  }

  const Digraph& condensation() const { return D; }

 private:
  int n;
  std::vector<int> id;
  std::vector<std::vector<int>> scc;
  Digraph D;
  mutable std::unordered_map<long long, std::vector<int>> E;

  long long hash(int i, int j) const { return i * scc.size() + j; }
};
