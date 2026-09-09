#pragma once

#include <bit>
#include <cassert>
#include <concepts>
#include <string>
#include <vector>

#include "algebra/group.hpp"
#include "algebra/monoid.hpp"

template <CommutativeMonoid M>
class FenwickTree {
 public:
  explicit FenwickTree(int n): n{n}, a(n + 1, M::identity()) {}

  template <typename T>
    requires std::constructible_from<M, const T&>
  explicit FenwickTree(const std::vector<T>& a): n(a.size()) {
    this->a.reserve(n + 1);
    this->a.push_back(M::identity());
    for (const auto& x: a) {
      this->a.emplace_back(x);
    }
    for (auto i = 1; i <= n; ++i) {
      auto j = i + (i & -i);
      if (j <= n) (this->a)[j] = (this->a)[j] * (this->a)[i];
    }
  }

  int size() const { return n; }

  M get(int i) const
    requires Group<M>
  {
    assert(0 <= i && i < n);
    return fold(i, i + 1);
  }

  template <typename T>
    requires Group<M> && std::constructible_from<M, const T&>
  void set(int i, const T& v) {
    assert(0 <= i && i < n);
    apply(i, get(i).inverse() * M{v});
  }

  template <typename T>
    requires std::constructible_from<M, const T&>
  void apply(int i, const T& v) {
    assert(0 <= i && i < n);
    M m{v};
    for (++i; i < n + 1; i += i & -i) a[i] = a[i] * m;
  }

  M fold() const { return fold(n); }

  M fold(int r) const {
    assert(0 <= r && r <= n);
    M res = M::identity();
    for (; r > 0; r -= r & -r) res = a[r] * res;
    return res;
  }

  M fold(int l, int r) const
    requires Group<M>
  {
    assert(0 <= l && l <= r && r <= n);
    return fold(l).inverse() * fold(r);
  }

  template <typename F>
    requires std::predicate<F&, M>
  int max_right(F f) const {
    assert(f(M::identity()));
    int x = 0;
    M cum = M::identity();
    for (int k = std::bit_floor<unsigned int>(n); k > 0; k >>= 1) {
      if (x + k <= n && f(cum * a[x + k])) {
        cum = cum * a[x + k];
        x += k;
      }
    }
    return x;
  }

  template <typename F>
    requires Group<M> && std::predicate<F&, M>
  int max_right(int l, F f) const {
    assert(0 <= l && l <= n);
    assert(f(M::identity()));
    int x = 0;
    M inv = fold(l).inverse(), cum = M::identity();
    for (int k = std::bit_floor<unsigned int>(n); k > 0; k >>= 1) {
      if (x + k <= l || (x + k <= n && f(inv * cum * a[x + k]))) {
        cum = cum * a[x + k];
        x += k;
      }
    }
    return x;
  }

  template <typename F>
    requires Group<M> && std::predicate<F&, M>
  int min_left(int r, F f) const {
    assert(0 <= r && r <= n);
    assert(f(M::identity()));

    M total = fold(r);
    if (f(total)) return 0;

    int x = 0;
    M cum = M::identity();
    for (int k = std::bit_floor<unsigned int>(r); k > 0; k >>= 1) {
      if (x + k <= r && !f((cum * a[x + k]).inverse() * total)) {
        cum = cum * a[x + k];
        x += k;
      }
    }
    return x + 1;
  }

  friend std::string pretty(const FenwickTree& F) {
    if constexpr (Group<M> && requires(const M& x) {
                    { pretty(x) } -> std::same_as<std::string>;
                  }) {
      std::string s = "[";
      for (auto i = 0; i < F.size(); ++i) {
        s += (i == 0 ? "" : ", ") + pretty(F.get(i));
      }
      s += "]";
      return s;
    }
    return "[" + std::to_string(F.size()) + " element(s)]";
  }

 private:
  int n;
  std::vector<M> a;
};
