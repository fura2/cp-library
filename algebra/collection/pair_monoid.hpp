#pragma once

#include <concepts>
#include <string>
#include <utility>

#include "algebra/monoid.hpp"

template <Monoid M, Monoid N>
class PairMonoid {
 public:
  PairMonoid(): m{M::identity()}, n{N::identity()} {}

  template <typename T, typename U>
    requires std::constructible_from<M, T&&> && std::constructible_from<N, U&&>
  PairMonoid(T&& x, U&& y): m(std::forward<T>(x)), n(std::forward<U>(y)) {}

  friend PairMonoid operator*(const PairMonoid& p, const PairMonoid& q) {
    return PairMonoid{p.m * q.m, p.n * q.n};
  }
  static PairMonoid identity() { return PairMonoid{}; }

  const M& first() const { return m; }
  M& first() { return m; }
  const N& second() const { return n; }
  N& second() { return n; }

  friend std::string pretty(const PairMonoid& p)
    requires requires(const M& m) {
      { pretty(m) } -> std::same_as<std::string>;
    } && requires(const N& n) {
      { pretty(n) } -> std::same_as<std::string>;
    }
  {
    return "(" + pretty(p.m) + ", " + pretty(p.n) + ")";
  }

 private:
  M m;
  N n;
};
