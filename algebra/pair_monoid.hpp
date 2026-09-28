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

  template <typename T = M, typename U = N>
    requires std::constructible_from<M, T&&> && std::constructible_from<N, U&&>
  PairMonoid(T&& x, U&& y)
      : first(std::forward<T>(x)), second(std::forward<U>(y)) {}

  template <typename T = M, typename U = N>
    requires std::constructible_from<M, const T&> &&
                 std::constructible_from<N, const U&>
  PairMonoid(const std::pair<T, U>& p): first(p.first), second(p.second) {}

  template <typename T = M, typename U = N>
    requires std::constructible_from<M, T&&> && std::constructible_from<N, U&&>
  PairMonoid(std::pair<T, U>&& p)
      : first(std::forward<T>(p.first)), second(std::forward<U>(p.second)) {}

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
