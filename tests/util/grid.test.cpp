#include "util/grid.hpp"

#include <cassert>
#include <utility>
#include <vector>

#include "tests/support/io.hpp"

template <typename R>
std::vector<std::pair<int, int>> cells(const R& range) {
  return {range.begin(), range.end()};
}

int main() {
  Grid empty;
  assert(empty.height() == 0 && empty.width() == 0);
  assert(!empty.contains(0, 0) && empty.count('.') == 0);
  empty.transpose();
  assert(empty.height() == 0 && empty.width() == 0);
  const Grid single{1, 1};
  assert(cells(single.neighbors4(0, 0)).empty());
  assert(cells(single.neighbors8(0, 0)).empty());
  Grid g{3, 3};
  assert(g.count('.') == 9);
  assert(g.contains(2, 2) && !g.contains(3, 0) && !g.contains(-1, 0));
  assert(!g.contains(0, -1) && !g.contains(0, 3));
  assert((cells(g.neighbors4(1, 1)) ==
          std::vector<std::pair<int, int>>{{1, 2}, {0, 1}, {1, 0}, {2, 1}}));
  assert((cells(g.neighbors4(0, 0)) ==
          std::vector<std::pair<int, int>>{{0, 1}, {1, 0}}));
  assert((cells(g.neighbors8(0, 0)) ==
          std::vector<std::pair<int, int>>{{0, 1}, {1, 0}, {1, 1}}));
  assert(cells(g.neighbors8(1, 1)).size() == 8);
  g[1][1] = '#';
  assert(g.count('#') == 1 && g.count('.') == 8);
  TestIO io{"abc def"};
  auto rectangle = input<Grid>(2, 3);
  rectangle.transpose();
  assert(rectangle.height() == 3 && rectangle.width() == 2);
  assert(rectangle[0] == "ad" && rectangle[1] == "be" && rectangle[2] == "cf");
  assert(pretty(rectangle) == "\nad\nbe\ncf");
  output(rectangle);
  assert(io.out.str() == "ad\nbe\ncf\n");
  rectangle.transpose();
  assert(rectangle[0] == "abc" && rectangle[1] == "def");
}
