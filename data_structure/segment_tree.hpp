#pragma once

#include <bit>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <vector>

#include "algebra/monoid.hpp"

template <Monoid M>
class SegmentTree {
 public:
  explicit SegmentTree(std::size_t n)
      : n{n},
        dep{n == 0 ? 0 : std::bit_width(n - 1)},
        a(1uz << (dep + 1), M::identity()) {}

  template <typename T>
    requires std::constructible_from<M, const T&>
  explicit SegmentTree(const std::vector<T>& a)
      : n{a.size()},
        dep{n == 0 ? 0 : std::bit_width(n - 1)},
        a(1uz << (dep + 1), M::identity()) {
    for (auto i = 0uz; i < n; ++i) {
      this->a[(1uz << dep) + i] = M{a[i]};
    }
    for (auto i = (1uz << dep) - 1; i > 0; --i) {
      this->a[i] = this->a[i << 1] * this->a[(i << 1) | 1];
    }
  }

  std::size_t size() const { return n; }

  const M& get(std::size_t i) const {
    assert(i < n);
    return a[(1uz << dep) + i];
  }

  template <typename T>
    requires std::constructible_from<M, const T&>
  void set(std::size_t i, const T& v) {
    assert(i < n);
    i += 1uz << dep;
    a[i] = M{v};
    while (i > 1) {
      i >>= 1;
      a[i] = a[i << 1] * a[(i << 1) | 1];
    }
  }

  M fold() const { return fold(0, n); }

  M fold(std::size_t l, std::size_t r) const {
    assert(l <= r && r <= n);
    M lcum = M::identity(), rcum = M::identity();
    l += 1uz << dep;
    r += 1uz << dep;
    while (l < r) {
      if (l & 1) lcum = lcum * a[l++];
      if (r & 1) rcum = a[--r] * rcum;
      l >>= 1;
      r >>= 1;
    }
    return lcum * rcum;
  }

  // // TODO
  // template <typename F>
  // std::size_t max_right(std::size_t l, F f) const
  //   requires std::predicate<F&, M>
  // {
  //   assert(f(M::identity()));
  //   std::size_t x = 0;
  //   return x;
  // }

  // template <typename F>
  // std::size_t min_left(std::size_t r, F f) const
  //   requires std::predicate<F&, M>
  // {
  //   assert(f(M::identity()));
  //   std::size_t x = 0;
  //   return x + 1;
  // }

  friend std::string pretty(const SegmentTree& S) {
    if constexpr (requires(const M& x) {
                    { pretty(x) } -> std::same_as<std::string>;
                  }) {
      std::string s = "[";
      for (auto i = 0uz; i < S.size(); ++i) {
        s += (i == 0 ? "" : ", ") + pretty(S.get(i));
      }
      s += "]";
      return s;
    }
    return "[" + std::to_string(S.size()) + " element(s)]";
  }

 private:
  std::size_t n;
  int dep;
  std::vector<M> a;
};
