#include "graph/odd_cycle.hpp"

#include <cassert>
#include <set>

#include "graph/graph.hpp"
#include "graph/weighted_graph.hpp"

template <typename G>
void check(const G& g, bool expected) {
  const auto [found, cycle] = odd_cycle(g);
  assert(found == expected);
  if (!found) {
    assert(cycle.empty());
    return;
  }
  assert(cycle.size() % 2 == 1);
  std::set<int> vertices;
  for (int i = 0; i < static_cast<int>(cycle.size()); ++i) {
    const auto& e = cycle[i];
    assert(e.to == cycle[(i + 1) % cycle.size()].from);
    assert(vertices.insert(e.from).second);
    assert(0 <= e.id && e.id < g.num_edges());
    const auto& original = g.edge(e.id);
    assert((original.from == e.from && original.to == e.to) ||
           (original.from == e.to && original.to == e.from));
    if constexpr (requires { e.cost; }) assert(e.cost == original.cost);
  }
}

int main() {
  check(Graph{0}, false);
  check(Graph{1}, false);
  Graph path{4};
  path.add_edge(0, 1);
  path.add_edge(1, 2);
  path.add_edge(2, 3);
  check(path, false);
  path.add_edge(3, 0);
  check(path, false);  // Even cycle.
  path.add_edge(0, 2);
  check(path, true);
  Graph parallel{2};
  parallel.add_edge(0, 1);
  parallel.add_edge(0, 1);
  check(parallel, false);
  Graph loop{2};
  loop.add_edge(1, 1);
  check(loop, true);
  WeightedGraph<int> disconnected{6};
  disconnected.add_edge(0, 1, 1);
  disconnected.add_edge(3, 4, 2);
  disconnected.add_edge(4, 5, 3);
  disconnected.add_edge(5, 3, -4);
  check(disconnected, true);
}
