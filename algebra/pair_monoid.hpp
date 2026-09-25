#pragma once

#include <concepts>
#include <string>
#include <utility>

#include "algebra/monoid.hpp"

template <Monoid M, Monoid N>
class PairMonoid {
 public:
  M first;
  N second;

  PairMonoid(): first{M::identity()}, second{N::identity()} {}

  template <typename T, typename U>
    requires std::constructible_from<M, T&&> && std::constructible_from<N, U&&>
  PairMonoid(T&& x, U&& y)
      : first(std::forward<T>(x)), second(std::forward<U>(y)) {}

  friend PairMonoid operator*(const PairMonoid& p, const PairMonoid& q) {
    return PairMonoid{p.first * q.first, p.second * q.second};
  }
  static PairMonoid identity() { return PairMonoid{}; }

  friend std::string pretty(const PairMonoid& p)
    requires requires(const M& m) {
      { pretty(m) } -> std::same_as<std::string>;
    } && requires(const N& n) {
      { pretty(n) } -> std::same_as<std::string>;
    }
  {
    return "(" + pretty(p.first) + ", " + pretty(p.second) + ")";
  }
};
