// clang-format off
#include "template/debug.hpp"
#include "util/grid.hpp"
// clang-format on

#include <cassert>
#include <string>
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
  TestIO io{"abc def ab cd #"};
  const auto rectangle = input<Grid>(2, 3);
  auto transposed = rectangle.transpose();
  assert(transposed.height() == 3 && transposed.width() == 2);
  assert(transposed[0] == "ad" && transposed[1] == "be" &&
         transposed[2] == "cf");
  assert(pretty(transposed) == "\nad\nbe\ncf");
  output(transposed);
  assert(io.out.str() == "ad\nbe\ncf\n");
  transposed = transposed.transpose();
  assert(transposed.height() == 2 && transposed.width() == 3);
  assert(transposed[0] == "abc" && transposed[1] == "def");

  // rotate() returns a grid rotated 90 degrees counterclockwise.
  auto rotated = rectangle.rotate();
  assert(rotated.height() == 3 && rotated.width() == 2);
  assert(rotated[0] == "cf" && rotated[1] == "be" && rotated[2] == "ad");
  rotated = rotated.rotate().rotate().rotate();
  assert(rotated.height() == 2 && rotated.width() == 3);
  assert(rotated[0] == "abc" && rotated[1] == "def");
  assert(rectangle.height() == 2 && rectangle.width() == 3);
  assert(rectangle[0] == "abc" && rectangle[1] == "def");

  for (auto [h, w]: {std::pair{0, 0}, {0, 3}, {2, 0}, {1, 1}, {1, 3}, {3, 1}}) {
    Grid border{h, w};
    for (int i = 0; i < h; ++i) border[i] = std::string(w, '#');
    const auto t = border.transpose();
    const auto r = border.rotate();
    assert(t.height() == w && t.width() == h && t.count('#') == h * w);
    assert(r.height() == w && r.width() == h && r.count('#') == h * w);
  }

  const auto empty_input = input<Grid>(0);
  assert(empty_input.height() == 0 && empty_input.width() == 0);
  const auto square = input<Grid>(2);
  assert(square.height() == 2 && square.width() == 2);
  assert(square[0] == "ab" && square[1] == "cd");
  const auto one_cell = input<Grid>(1);
  assert(one_cell.height() == 1 && one_cell.width() == 1);
  assert(one_cell[0] == "#");
}
