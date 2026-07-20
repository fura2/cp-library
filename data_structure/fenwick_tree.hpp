#pragma once

#include <cassert>
#include <concepts>
#include <cstddef>
#include <vector>

#include "algebra/group.hpp"
#include "algebra/monoid.hpp"

template <CommutativeMonoid M>
class FenwickTree {
 public:
  FenwickTree(std::size_t n = 0): n{n}, a(n + 1, M::identity()) {}

  template <typename T>
    requires std::constructible_from<M, const T&>
  FenwickTree(const std::vector<T>& a): n{a.size()} {
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
    requires AbelianGroup<M>
  {
    assert(l <= r && r <= n);
    return fold(l).inverse() * fold(r);
  }
  /*
    int lower_bound(G val) const {
      if (!(G{} < val)) return 0;
      std::size_t x = 0, k;
      for (k = 1; k <= a.size(); k <<= 1);
      for (k >>= 1; k > 0; k >>= 1)
        if (x + k <= a.size() && a[x + k - 1] < val) val -= a[x + k - 1], x +=
    k; return x;
    }
    int upper_bound(G val) const {
      if (val < G{}) return 0;
      std::size_t x = 0, k;
      for (k = 1; k <= a.size(); k <<= 1);
      for (k >>= 1; k > 0; k >>= 1)
        if (x + k <= a.size() && !(val < a[x + k - 1]))
          val -= a[x + k - 1], x += k;
      return x;
    }
  */

  friend std::string pretty(const FenwickTree& f) {
    if constexpr (AbelianGroup<M> && requires(const M& x) {
                    { pretty(x) } -> std::same_as<std::string>;
                  }) {
      std::string s = "[";
      for (auto i = 0uz; i < f.size(); ++i) {
        s += (i == 0 ? "" : ", ") + pretty(f.fold(i, i + 1));
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
