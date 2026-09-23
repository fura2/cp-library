#pragma once

#include <concepts>
#include <string>
#include <utility>

#include "algebra/group.hpp"

template <Group G, Group H>
class PairGroup {
 public:
  PairGroup(): g{G::identity()}, h{H::identity()} {}

  template <typename T, typename U>
    requires std::constructible_from<G, T&&> && std::constructible_from<H, U&&>
  PairGroup(T&& x, U&& y): g(std::forward<T>(x)), h(std::forward<U>(y)) {}

  friend PairGroup operator*(const PairGroup& p, const PairGroup& q) {
    return PairGroup{p.g * q.g, p.h * q.h};
  }
  static PairGroup identity() { return PairGroup{}; }

  PairGroup inverse() const { return PairGroup{g.inverse(), h.inverse()}; }

  const G& first() const { return g; }
  G& first() { return g; }
  const H& second() const { return h; }
  H& second() { return h; }

  friend std::string pretty(const PairGroup& p)
    requires requires(const G& g) {
      { pretty(g) } -> std::same_as<std::string>;
    } && requires(const H& h) {
      { pretty(h) } -> std::same_as<std::string>;
    }
  {
    return "(" + pretty(p.g) + ", " + pretty(p.h) + ")";
  }

 private:
  G g;
  H h;
};
