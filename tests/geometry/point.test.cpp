#include "geometry/point.hpp"

#include <cassert>

#include "tests/support/io.hpp"

void check(const Point& p, long long x, long long y) {
  assert(p.x == x && p.y == y);
}

int main() {
  const Point p{2, -3}, q{-1, 4};
  check(Point{}, 0, 0);
  check(p + q, 1, 1);
  check(p - q, 3, -7);
  check(-p, -2, 3);
  check(p * 3, 6, -9);
  check(-2 * p, -4, 6);
  Point r = p;
  r += q;
  r -= p;
  r *= 2;
  check(r, -2, 8);
  assert(dot(p, q) == -14);
  assert(cross(p, q) == 5);
  assert(cross(q, p) == -5);
  assert(cross(p, p) == 0);
  assert(dot(Point{100'000, 0}, Point{100'000, 0}) == 10'000'000'000LL);
  assert(pretty(p) == "(2, -3)");
  TestIO io{"7 -8"};
  const auto read = input<Point>();
  check(read, 7, -8);
  output(read);
  assert(io.out.str() == "7 -8\n");
}
