#pragma once

#include <concepts>
#include <string>
#include <utility>

#include "algebra/group.hpp"

template <Group G, Group H>
class PairGroup {
 public:
  G first;
  H second;

  PairGroup(): first{G::identity()}, second{H::identity()} {}

  template <typename T = G, typename U = H>
    requires std::constructible_from<G, T&&> && std::constructible_from<H, U&&>
  PairGroup(T&& x, U&& y)
      : first(std::forward<T>(x)), second(std::forward<U>(y)) {}

  template <typename T = G, typename U = H>
    requires std::constructible_from<G, const T&> &&
                 std::constructible_from<H, const U&>
  PairGroup(const std::pair<T, U>& p): first(p.first), second(p.second) {}

  template <typename T = G, typename U = H>
    requires std::constructible_from<G, T&&> && std::constructible_from<H, U&&>
  PairGroup(std::pair<T, U>&& p)
      : first(std::forward<T>(p.first)), second(std::forward<U>(p.second)) {}

  friend PairGroup operator*(const PairGroup& p, const PairGroup& q) {
    return PairGroup{p.first * q.first, p.second * q.second};
  }
  static PairGroup identity() { return PairGroup{}; }

  PairGroup inverse() const {
    return PairGroup{first.inverse(), second.inverse()};
  }

  friend std::string pretty(const PairGroup& p)
    requires requires(const G& g) {
      { pretty(g) } -> std::same_as<std::string>;
    } && requires(const H& h) {
      { pretty(h) } -> std::same_as<std::string>;
    }
  {
    return "(" + pretty(p.first) + ", " + pretty(p.second) + ")";
  }
};
