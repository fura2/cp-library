#pragma once

#include <string>
#include <utility>

#include "template/io.hpp"

struct Point {
  long long x, y;

  friend Point operator+(const Point& p, const Point& q) {
    return Point{p.x + q.x, p.y + q.y};
  }
  friend Point operator-(const Point& p, const Point& q) {
    return Point{p.x - q.x, p.y - q.y};
  }
  friend Point operator*(long long c, const Point& p) {
    return Point{c * p.x, c * p.y};
  }
  friend Point operator*(const Point& p, long long c) {
    return Point{p.x * c, p.y * c};
  }
  friend Point operator/(const Point& p, long long c) {
    return Point{p.x / c, p.y / c};
  }
  Point operator-() const { return Point{-x, -y}; }

  friend void output(const Point& p) { output(std::pair{p.x, p.y}); }

  friend std::string pretty(const Point& p) {
    return "(" + std::to_string(p.x) + ", " + std::to_string(p.y) + ")";
  }
};

template <>
struct Input<Point> {
  static Point read() { return Point{input<long long>(), input<long long>()}; }
};

inline constexpr long long dot(const Point& p, const Point& q) {
  return p.x * q.x + p.y * q.y;
}

inline constexpr long long cross(const Point& p, const Point& q) {
  return p.x * q.y - p.y * q.x;
}
