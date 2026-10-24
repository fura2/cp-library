#include "graph/bfs_01.hpp"

#include <cassert>
#include <concepts>
#include <vector>

#include "graph/digraph.hpp"
#include "graph/graph.hpp"
#include "graph/weighted_digraph.hpp"
#include "graph/weighted_graph.hpp"
#include "template/constant.hpp"

template <typename G>
concept CanBfs01 = requires(const G& g) {
  { bfs_01(g, 0) } -> std::same_as<std::vector<int>>;
};

static_assert(CanBfs01<WeightedGraph<int>>);
static_assert(CanBfs01<WeightedDigraph<long long>>);
static_assert(!CanBfs01<Graph>);
static_assert(!CanBfs01<Digraph>);
static_assert(!CanBfs01<int>);

template <typename G>
void check(const G& g, int s, std::vector<int> expected) {
  assert(bfs_01(g, s) == expected);
}

int main() {
  check(WeightedDigraph<int>{1}, 0, {0});
  check(WeightedDigraph<int>{3}, 2, {INF, INF, 0});

  WeightedDigraph<long long> directed{7};
  directed.add_edge(3, 0, 1);
  directed.add_edge(3, 1, 0);
  directed.add_edge(1, 2, 0);
  // A longer zero-cost path improves an already discovered vertex.
  directed.add_edge(2, 0, 0);
  directed.add_edge(0, 4, 1);
  directed.add_edge(4, 5, 1);
  directed.add_edge(2, 1, 0);  // Zero-cost cycle.
  directed.add_edge(2, 2, 0);
  directed.add_edge(4, 4, 1);
  directed.add_edge(3, 1, 1);  // Parallel edges with different costs.
  check(directed, 3, {0, 0, 0, 0, 1, 2, INF});
  check(directed, 5, {INF, INF, INF, INF, INF, 0, INF});

  WeightedGraph<int> undirected{5};
  undirected.add_edge(0, 1, 1);
  undirected.add_edge(1, 2, 0);
  undirected.add_edge(2, 3, 1);
  undirected.add_edge(0, 3, 1);
  check(undirected, 3, {1, 1, 1, 0, INF});
}
