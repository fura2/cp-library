#include "graph/strongly_connected_components.hpp"

#include <algorithm>
#include <cassert>
#include <type_traits>
#include <vector>

#include "graph/digraph.hpp"
#include "graph/graph.hpp"
#include "graph/weighted_digraph.hpp"
#include "graph/weighted_graph.hpp"

static_assert(
    std::is_constructible_v<StronglyConnectedComponents, const Digraph&>);
static_assert(std::is_constructible_v<StronglyConnectedComponents,
                                      const WeightedDigraph<int>&>);
static_assert(
    !std::is_convertible_v<const Digraph&, StronglyConnectedComponents>);
static_assert(!std::is_convertible_v<const WeightedDigraph<int>&,
                                     StronglyConnectedComponents>);
static_assert(
    !std::is_constructible_v<StronglyConnectedComponents, const Graph&>);
static_assert(!std::is_constructible_v<StronglyConnectedComponents,
                                       const WeightedGraph<int>&>);
static_assert(!std::is_constructible_v<StronglyConnectedComponents, int>);

template <typename G>
void check(const G& g, std::vector<std::vector<int>> expected) {
  const StronglyConnectedComponents scc{g};
  const int k = scc.size();
  assert(k == static_cast<int>(expected.size()));
  auto actual = scc.components();
  for (auto& component: actual) std::ranges::sort(component);
  for (auto& component: expected) std::ranges::sort(component);
  std::ranges::sort(actual);
  std::ranges::sort(expected);
  assert(actual == expected);

  for (int i = 0; i < k; ++i) {
    assert(scc.component(i) == scc.components()[i]);
    for (int u: scc.component(i)) assert(scc.component_id(u) == i);
  }

  // Check original edge IDs, including internal edges and absent pairs.
  std::vector expected_edges(k, std::vector<std::vector<int>>(k));
  for (int i = 0; i < g.num_edges(); ++i) {
    const auto& e = g.edge(i);
    expected_edges[scc.component_id(e.from)][scc.component_id(e.to)]
        .emplace_back(e.id);
  }

  const auto& dag = scc.condensation();
  assert(dag.num_vertices() == k);
  std::vector dag_edges(k, std::vector<int>(k));
  for (int i = 0; i < dag.num_edges(); ++i) {
    const auto& e = dag.edge(i);
    assert(0 <= e.from && e.from < k && 0 <= e.to && e.to < k);
    assert(e.from < e.to);  // Component IDs follow a topological order.
    ++dag_edges[e.from][e.to];
  }
  for (int i = 0; i < k; ++i) {
    for (int j = 0; j < k; ++j) {
      auto ids = scc.edge_ids(i, j);
      std::ranges::sort(ids);
      assert(ids == expected_edges[i][j]);
      // Condensation has one edge per connected pair, without self-loops.
      assert(dag_edges[i][j] == (i != j && !expected_edges[i][j].empty()));
    }
  }
}

int main() {
  check(Digraph{}, {});
  check(Digraph{1}, {{0}});
  check(Digraph{4}, {{0}, {1}, {2}, {3}});

  Digraph loops{1};
  loops.add_edge(0, 0);
  loops.add_edge(0, 0);
  check(loops, {{0}});

  Digraph path{4};
  path.add_edge(2, 0);
  path.add_edge(0, 3);
  path.add_edge(3, 1);
  check(path, {{0}, {1}, {2}, {3}});

  Digraph cycle{3};
  cycle.add_edge(2, 0);
  cycle.add_edge(0, 1);
  cycle.add_edge(1, 2);
  check(cycle, {{0, 1, 2}});

  Digraph g{8};
  g.add_edge(0, 3);
  g.add_edge(3, 5);
  g.add_edge(5, 0);
  g.add_edge(5, 4);
  g.add_edge(0, 1);  // Same component pair, different endpoints.
  g.add_edge(5, 4);  // Parallel edges retain separate original IDs.
  g.add_edge(1, 4);
  g.add_edge(4, 1);
  g.add_edge(4, 2);
  g.add_edge(1, 2);
  g.add_edge(3, 2);  // Keep transitive edges in the condensation.
  g.add_edge(6, 6);
  g.add_edge(2, 2);
  g.add_edge(4, 4);
  check(g, {{0, 3, 5}, {1, 4}, {2}, {6}, {7}});

  check(WeightedDigraph<int>{}, {});
  WeightedDigraph<int> weighted{6};
  weighted.add_edge(2, 0, -7);
  weighted.add_edge(0, 2, 3);
  weighted.add_edge(0, 3, 0);
  weighted.add_edge(2, 1, -5);
  weighted.add_edge(2, 1, 9);
  weighted.add_edge(1, 3, 4);
  weighted.add_edge(3, 1, -4);
  weighted.add_edge(3, 4, 2);
  weighted.add_edge(4, 4, -1);
  check(weighted, {{0, 2}, {1, 3}, {4}, {5}});
}
