#pragma once

#include <iterator>
#include <vector>

#include "graph/incidence_graph.hpp"

template <typename GraphT>
  requires IncidenceGraph<GraphT> && Undirected<GraphT>
std::vector<typename GraphT::edge_type> odd_cycle(const GraphT& G) {
  int n = G.num_vertices();

  std::vector<int> color(n, -1);
  std::vector<typename GraphT::edge_type> path, cycle;

  auto dfs = [&](auto&& dfs, int u, int c) -> bool {
    color[u] = c;
    for (const auto& e: G[u]) {
      int v = e.to;
      if (color[v] == c) {
        if (v != u) {
          auto it = std::prev(path.end());
          while (it->from != v) --it;
          cycle = decltype(cycle)(std::make_move_iterator(it),
                                  std::make_move_iterator(path.end()));
        }
        cycle.emplace_back(e);
        return true;
      }
      if (color[v] != -1) continue;

      path.emplace_back(e);
      if (dfs(dfs, v, 1 - c)) return true;
      path.pop_back();
    }
    return false;
  };

  for (int u = 0; u < n; ++u) {
    if (color[u] == -1 && dfs(dfs, u, 0)) return cycle;
  }
  return {};
}
