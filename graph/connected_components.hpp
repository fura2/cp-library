#pragma once

#include <queue>
#include <utility>
#include <vector>

#include "graph/incidence_graph.hpp"
template <typename GraphT>
  requires IncidenceGraph<GraphT> && Undirected<GraphT>
std::vector<std::vector<int>> connected_components(const GraphT& G) {
  int n = G.num_vertices();
  std::vector<std::vector<int>> res;
  std::vector<bool> vis(n);
  std::queue<int> Q;
  for (int u = 0; u < n; ++u) {
    if (vis[u]) continue;
    vis[u] = true;
    Q.emplace(u);
    std::vector<int> cc = {u};
    while (!Q.empty()) {
      int v = Q.front();
      Q.pop();
      for (const auto& e: G[v]) {
        if (!vis[e.to]) {
          vis[e.to] = true;
          Q.emplace(e.to);
          cc.emplace_back(e.to);
        }
      }
    }
    res.emplace_back(std::move(cc));
  }
  return res;
}
