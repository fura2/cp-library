#pragma once

#include <cassert>
#include <deque>
#include <utility>
#include <vector>

#include "graph/incidence_graph.hpp"
#include "template/constant.hpp"

template <typename GraphT>
  requires IncidenceGraph<GraphT> && Weighted<GraphT>
std::vector<int> bfs_01(const GraphT& G, int s) {
  int n = G.num_vertices();
  assert(0 <= s && s < n);
  std::vector<int> d(n, INF);
  std::deque<std::pair<int, int>> Q;
  d[s] = 0;
  Q.emplace_back(0, s);
  while (!Q.empty()) {
    auto [du, u] = Q.front();
    Q.pop_front();
    if (du > d[u]) continue;
    for (const auto& e: G[u]) {
      if (d[e.to] > d[u] + e.cost) {
        d[e.to] = d[u] + e.cost;
        if (e.cost == 0) {
          Q.emplace_front(d[e.to], e.to);
        }
        else {
          Q.emplace_back(d[e.to], e.to);
        }
      }
    }
  }
  return d;
}
