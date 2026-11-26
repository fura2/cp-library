#include "graph/weighted_tree.hpp"

#include <cassert>
#include <ranges>

#include "graph/incidence_graph.hpp"
#include "template/io.hpp"
#include "tests/support/io.hpp"

using Tree = WeightedTree<long long>;
static_assert(IncidenceGraph<Tree> && Weighted<Tree> && Undirected<Tree>);

void check_add_edge() {
  Tree tree{2};
  const long long cost = -5'000'000'000LL;
  assert(tree.add_edge(1, 0, cost) == 0);
  assert(tree.num_vertices() == 2 && tree.num_edges() == 1);
  assert(tree.degree(0) == 1 && tree.degree(1) == 1);

  const auto& edge = tree.edge(0);
  assert(edge.from == 1 && edge.to == 0 && edge.id == 0 && edge.cost == cost);
  for (int u = 0; u < 2; ++u) {
    assert(std::ranges::distance(tree[u]) == 1);
    const auto& e = *tree[u].begin();
    assert(e.from == u && e.to == 1 - u && e.id == edge.id && e.cost == cost);
  }
}

void check_input() {
  {
    TestIO io{"1 0 -7"};
    const auto single = input<Tree>(1);
    assert(single.num_vertices() == 1 && single.num_edges() == 0);
    assert(single.degree(0) == 0 && std::ranges::empty(single[0]));

    const auto tree = input<Tree>(2);
    const auto& e = tree.edge(0);
    assert(e.from == 1 && e.to == 0 && e.cost == -7);
  }
  {
    TestIO io{"2 3 5000000000\n1 2 0\n42"};
    const auto tree = input<Tree>(3, 1);
    assert(tree.num_vertices() == 3 && tree.num_edges() == 2);
    assert(tree.degree(1) == 2);
    const auto& first = tree.edge(0);
    assert(first.from == 1 && first.to == 2 && first.cost == 5'000'000'000LL);
    const auto& second = tree.edge(1);
    assert(second.from == 0 && second.to == 1 && second.cost == 0);
    assert(first.id == 0 && second.id == 1);
    assert(input<int>() == 42);
  }
}

int main() {
  check_add_edge();
  check_input();
}
