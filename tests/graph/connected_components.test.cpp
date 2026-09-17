#include "graph/connected_components.hpp"

#include <algorithm>
#include <cassert>
#include <vector>

#include "graph/graph.hpp"
#include "graph/weighted_graph.hpp"

template <typename G>
void check(const G& g, std::vector<std::vector<int>> expected) {
  auto actual = connected_components(g);
  for (auto& component: actual) std::ranges::sort(component);
  std::ranges::sort(actual);
  std::ranges::sort(expected);
  assert(actual == expected);
}

int main() {
  check(Graph{0}, {});
  check(Graph{1}, {{0}});
  check(Graph{3}, {{0}, {1}, {2}});
  Graph g{6};
  g.add_edge(0, 2);
  g.add_edge(2, 4);
  g.add_edge(0, 2);
  g.add_edge(1, 3);
  g.add_edge(5, 5);
  check(g, {{0, 2, 4}, {1, 3}, {5}});
  WeightedGraph<int> weighted{3};
  weighted.add_edge(2, 0, -9);
  check(weighted, {{0, 2}, {1}});
}
