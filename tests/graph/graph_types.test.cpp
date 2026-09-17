// Declare pretty overloads before templates that use them.
// clang-format off
#include "template/debug.hpp"
// clang-format on

#include <cassert>
#include <ranges>
#include <type_traits>

#include "graph/digraph.hpp"
#include "graph/edge.hpp"
#include "graph/graph.hpp"
#include "graph/incidence_graph.hpp"
#include "graph/weighted_digraph.hpp"
#include "graph/weighted_edge.hpp"
#include "graph/weighted_graph.hpp"
#include "tests/support/io.hpp"

static_assert(IncidenceGraph<Graph> && Undirected<Graph> && !Directed<Graph>);
static_assert(IncidenceGraph<Digraph> && Directed<Digraph> &&
              !Undirected<Digraph>);
static_assert(IncidenceGraph<WeightedGraph<long long>>);
static_assert(IncidenceGraph<WeightedDigraph<long long>>);
static_assert(!IncidenceGraph<int>);

template <typename G>
int add(G& g, int u, int v, int cost) {
  if constexpr (std::same_as<typename G::edge_type, Edge>) {
    return g.add_edge(u, v);
  }
  else {
    return g.add_edge(u, v, cost);
  }
}

template <typename G>
void check_graph() {
  const G empty{0};
  assert(empty.num_vertices() == 0 && empty.num_edges() == 0);
  G g{3, 4};
  assert(g.num_vertices() == 3 && g.num_edges() == 0);
  assert(add(g, 0, 1, -7) == 0);
  assert(add(g, 0, 1, 8) == 1);  // Parallel edges have different IDs.
  assert(add(g, 1, 1, 3) == 2);  // Self-loop.
  assert(g.num_edges() == 3);
  assert(g.edge(0).from == 0 && g.edge(0).to == 1 && g.edge(0).id == 0);
  assert(g.edge(1).id == 1 && g.edge(2).from == 1 && g.edge(2).to == 1);
  if constexpr (requires { g.edge(0).cost; }) {
    assert(g.edge(0).cost == -7 && g.edge(1).cost == 8);
  }
  assert(std::ranges::distance(g[0]) == 2);
  assert(std::ranges::distance(g[1]) == (G::directed ? 1 : 4));
  assert(std::ranges::empty(g[2]));
  for (const auto& e: g[1]) {
    assert(e.from == 1);
    if (e.id < 2) {
      assert(!G::directed && e.to == 0);
      if constexpr (requires { e.cost; }) assert(e.cost == g.edge(e.id).cost);
    }
    else {
      assert(e.id == 2 && e.to == 1);
    }
  }
  // Exercise the default input offset separately from 1-based input.
  {
    TestIO io{"0 2 9"};
    const auto read = input<G>(3, 1);
    assert(read.edge(0).from == 0 && read.edge(0).to == 2);
  }
  {
    const bool weighted = !std::same_as<typename G::edge_type, Edge>;
    TestIO io{weighted ? "1 2 -5 3 1 9" : "1 2 3 1"};
    const auto read = input<G>(3, 2, 1);
    assert(read.num_edges() == 2);
    assert(read.edge(0).from == 0 && read.edge(0).to == 1);
    assert(read.edge(1).from == 2 && read.edge(1).to == 0);
    if constexpr (requires { read.edge(0).cost; }) {
      assert(read.edge(0).cost == -5 && read.edge(1).cost == 9);
    }
  }
}

int main() {
  const Edge edge{2, 3};
  assert(edge.from == 2 && edge.to == 3 && edge.id == -1);
  assert(pretty(edge) == "(2, 3)");
  const WeightedEdge<long long> weighted{2, 3, -7};
  assert(weighted.cost == -7 && weighted.id == -1);
  assert(pretty(weighted) == "(2, 3; -7)");
  check_graph<Graph>();
  check_graph<Digraph>();
  check_graph<WeightedGraph<long long>>();
  check_graph<WeightedDigraph<long long>>();
}
