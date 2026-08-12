#pragma once

#include <bit>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <vector>

#include "algebra/group.hpp"
#include "algebra/monoid.hpp"

template <CommutativeMonoid M>
class FenwickTree {
 public:
  explicit FenwickTree(std::size_t n): n{n}, a(n + 1, M::identity()) {}

  template <typename T>
    requires std::constructible_from<M, const T&>
  explicit FenwickTree(const std::vector<T>& a): n{a.size()} {
    this->a.reserve(n + 1);
    this->a.push_back(M::identity());
    for (const auto& x: a) {
      this->a.emplace_back(x);
    }
    for (auto i = 1uz; i <= n; ++i) {
      auto j = i + (i & -i);
      if (j <= n) (this->a)[j] = (this->a)[j] * (this->a)[i];
    }
  }

  std::size_t size() const { return n; }

  M get(std::size_t i) const
    requires Group<M>
  {
    assert(i < n);
    return fold(i, i + 1);
  }

  template <typename T>
    requires Group<M> && std::constructible_from<M, const T&>
  void set(std::size_t i, const T& v) {
    assert(i < n);
    apply(i, get(i).inverse() * M{v});
  }

  template <typename T>
    requires std::constructible_from<M, const T&>
  void apply(std::size_t i, const T& v) {
    assert(i < n);
    M m{v};
    for (++i; i < n + 1; i += i & -i) a[i] = a[i] * m;
  }

  M fold() const { return fold(n); }

  M fold(std::size_t r) const {
    assert(r <= n);
    M res = M::identity();
    for (; r > 0; r -= r & -r) res = a[r] * res;
    return res;
  }

  M fold(std::size_t l, std::size_t r) const
    requires Group<M>
  {
    assert(l <= r);
    assert(r <= n);
    return fold(l).inverse() * fold(r);
  }

  template <typename F>
  std::size_t max_right(F f) const
    requires std::predicate<F&, M>
  {
    assert(f(M::identity()));
    std::size_t x = 0;
    M cum = M::identity();
    for (auto k = std::bit_floor(n); k > 0; k >>= 1) {
      if (x + k <= n && f(cum * a[x + k])) {
        cum = cum * a[x + k];
        x += k;
      }
    }
    return x;
  }

  template <typename F>
  std::size_t max_right(std::size_t l, F f) const
    requires Group<M> && std::predicate<F&, M>
  {
    assert(l <= n);
    assert(f(M::identity()));
    std::size_t x = 0;
    M inv = fold(l).inverse(), cum = M::identity();
    for (auto k = std::bit_floor(n); k > 0; k >>= 1) {
      if (x + k <= l || (x + k <= n && f(inv * cum * a[x + k]))) {
        cum = cum * a[x + k];
        x += k;
      }
    }
    return x;
  }

  template <typename F>
  std::size_t min_left(std::size_t r, F f) const
    requires Group<M> && std::predicate<F&, M>
  {
    assert(r <= n);
    assert(f(M::identity()));

    M total = fold(r);
    if (f(total)) return 0;

    std::size_t x = 0;
    M cum = M::identity();
    for (auto k = std::bit_floor(r); k > 0; k >>= 1) {
      if (x + k <= r && !f((cum * a[x + k]).inverse() * total)) {
        cum = cum * a[x + k];
        x += k;
      }
    }
    return x + 1;
  }

  friend std::string pretty(const FenwickTree& f) {
    if constexpr (Group<M> && requires(const M& x) {
                    { pretty(x) } -> std::same_as<std::string>;
                  }) {
      std::string s = "[";
      for (auto i = 0uz; i < f.size(); ++i) {
        s += (i == 0 ? "" : ", ") + pretty(f.get(i));
      }
      s += "]";
      return s;
    }
    return "[" + std::to_string(f.size()) + " element(s)]";
  }

 private:
  std::size_t n;
  std::vector<M> a;
};
