#include "graph/degree_sequence.hpp"

#include <algorithm>
#include <cassert>
#include <limits>
#include <numeric>
#include <set>
#include <utility>
#include <vector>

void check(const std::vector<int>& d, bool expected) {
  assert(erdos_gallai(d) == expected);
  if (!expected) return;

  const auto g = havel_hakimi(d);
  assert(g.num_vertices() == static_cast<int>(d.size()));
  assert(2LL * g.num_edges() == std::accumulate(d.begin(), d.end(), 0LL));
  for (int u = 0; u < g.num_vertices(); ++u) {
    assert(g.degree(u) == d[u]);
  }

  // The realization need not be unique, but it must be a simple graph.
  std::set<std::pair<int, int>> edges;
  for (int i = 0; i < g.num_edges(); ++i) {
    const auto& e = g.edge(i);
    assert(0 <= e.from && e.from < g.num_vertices());
    assert(0 <= e.to && e.to < g.num_vertices());
    assert(e.from != e.to);
    assert(
        edges.emplace(std::min(e.from, e.to), std::max(e.from, e.to)).second);
  }
}

int main() {
  check({}, true);
  check({0}, true);
  check({0, 0, 0, 0}, true);
  check({1, 1}, true);     // The Erdos-Gallai boundary p must advance with k.
  check({2, 2, 2}, true);  // Reductions create a previously absent degree.
  check({4, 2, 2, 1, 1}, true);        // Skip an empty degree class.
  check({6, 4, 4, 2, 2, 2, 2}, true);  // Degree 2 disappears and reappears.
  check({2, 2, 2, 2, 2}, true);  // Partially consume an equal-degree class.
  check({4, 4, 4, 4, 4}, true);  // Complete graph.
  check({1, 0, 3, 1, 1}, true);  // Preserve vertex labels; includes an isolate.
  check({1, 0, 1, 0, 1, 1}, true);  // Disconnected matching and isolates.

  check({-1, 1}, false);
  check({1}, false);
  check({2, 0}, false);        // A degree equal to n is invalid.
  check({1, 1, 1}, false);     // Odd total degree.
  check({2, 0, 0}, false);     // Even total, but fails the k = 1 inequality.
  check({3, 3, 1, 1}, false);  // Passes k = 1, but fails at k = 2.

  // Reject out-of-range values before adding them; also run with UBSan.
  const int hi = std::numeric_limits<int>::max();
  const int lo = std::numeric_limits<int>::min();
  check({hi, hi, 0, 0}, false);
  check({lo, lo, 0, 0}, false);

  // Both the degree sum and k * (k - 1) require 64 bits. Only test existence:
  // explicitly constructing this complete graph would require billions of
  // edges.
  assert(erdos_gallai(std::vector<int>(65536, 65535)));
}
