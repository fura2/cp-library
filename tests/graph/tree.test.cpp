#include "graph/tree.hpp"

#include <algorithm>
#include <cassert>
#include <ranges>
#include <utility>
#include <vector>

#include "graph/incidence_graph.hpp"
#include "template/io.hpp"
#include "tests/support/io.hpp"

static_assert(IncidenceGraph<Tree> && Undirected<Tree> && !Directed<Tree>);
static_assert(!Weighted<Tree>);

void check_centers(const Tree& tree, std::vector<int> expected, int diameter) {
  auto [centers, actual_diameter] = tree.centers();
  std::ranges::sort(centers);
  std::ranges::sort(expected);
  assert(centers == expected);
  assert(actual_diameter == diameter);
}

Tree make_path(int n) {
  Tree tree{n};
  for (int u = 1; u < n; ++u) tree.add_edge(u - 1, u);
  return tree;
}

void check_basic_operations() {
  const Tree single{1};
  assert(single.num_vertices() == 1 && single.num_edges() == 0);
  assert(single.degree(0) == 0 && std::ranges::empty(single[0]));
  check_centers(single, {0}, 0);

  Tree tree{5};
  const Tree& view = tree;
  assert(view.num_vertices() == 5 && view.num_edges() == 0);
  assert(view.degree(3) == 0 && std::ranges::empty(view[3]));
  assert(tree.add_edge(0, 3) == 0);
  // An added edge is accessible before the tree is complete.
  assert(view.num_edges() == 1);
  assert(view.edge(0).from == 0 && view.edge(0).to == 3 &&
         view.edge(0).id == 0);
  assert(tree.add_edge(3, 1) == 1);
  assert(tree.add_edge(3, 2) == 2);
  assert(tree.add_edge(1, 4) == 3);
  assert(view.num_vertices() == 5 && view.num_edges() == 4);
  assert(view.degree(0) == 1 && view.degree(1) == 2 && view.degree(3) == 3);
  assert(view.edge(3).from == 1 && view.edge(3).to == 4 &&
         view.edge(3).id == 3);
  std::vector<std::pair<int, int>> neighbors;
  for (const auto& e: view[3]) {
    assert(e.from == 3);
    neighbors.emplace_back(e.to, e.id);
  }
  std::ranges::sort(neighbors);
  assert(
      (neighbors == std::vector<std::pair<int, int>>{{0, 0}, {1, 1}, {2, 2}}));
  check_centers(view, {1, 3}, 3);
}

void check_center_shapes() {
  check_centers(make_path(2), {0, 1}, 1);
  check_centers(make_path(5), {2}, 4);
  check_centers(make_path(6), {2, 3}, 5);

  // Multiple diameter paths share a center whose label is not zero.
  Tree star{6};
  for (int u = 0; u < 6; ++u) {
    if (u != 4) star.add_edge(u, 4);
  }
  check_centers(star, {4}, 2);

  // Vertex 0 lies on a short branch, outside every diameter path.
  // The unique diameter is 3 -- 2 -- 1 -- 4 -- 5 -- 6.
  Tree branched{7};
  branched.add_edge(2, 3);
  branched.add_edge(1, 2);
  branched.add_edge(1, 0);
  branched.add_edge(4, 1);
  branched.add_edge(5, 4);
  branched.add_edge(6, 5);
  check_centers(branched, {1, 4}, 5);
}

void check_input() {
  {
    TestIO io{"42"};
    const auto tree = input<Tree>(1);
    assert(tree.num_vertices() == 1 && tree.num_edges() == 0);
    assert(input<int>() == 42);  // A singleton consumes no edges.
  }
  {
    TestIO io{"2 0 0 1 42"};
    const auto tree = input<Tree>(3);
    assert(tree.num_vertices() == 3 && tree.num_edges() == 2);
    assert(tree.edge(0).from == 2 && tree.edge(0).to == 0);
    assert(tree.edge(1).from == 0 && tree.edge(1).to == 1);
    assert(input<int>() == 42);
  }
  {
    TestIO io{"3 1 1 2 42"};
    const auto tree = input<Tree>(3, 1);
    assert(tree.num_vertices() == 3 && tree.num_edges() == 2);
    assert(tree.edge(0).from == 2 && tree.edge(0).to == 0);
    assert(tree.edge(1).from == 0 && tree.edge(1).to == 1);
    assert(input<int>() == 42);
    check_centers(tree, {0}, 2);
  }
}

int main() {
  check_basic_operations();
  check_center_shapes();
  check_input();

  // Regression: recursive DFS can exhaust an 8 MiB stack on a long valid path.
  const int n = 200000;
  check_centers(make_path(n), {n / 2 - 1, n / 2}, n - 1);
}
