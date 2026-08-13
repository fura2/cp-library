#pragma once

#include <bit>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <string>
#include <vector>

#include "algebra/monoid.hpp"

template <Monoid M>
class SegmentTree {
 public:
  explicit SegmentTree(std::size_t n)
      : n{n}, sz{std::bit_ceil(n)}, a(2 * sz, M::identity()) {}

  template <typename T>
    requires std::constructible_from<M, const T&>
  explicit SegmentTree(const std::vector<T>& a)
      : n{a.size()}, sz{std::bit_ceil(n)}, a(2 * sz, M::identity()) {
    for (auto i = 0uz; i < n; ++i) {
      this->a[sz + i] = M{a[i]};
    }
    for (auto i = sz - 1; i > 0; --i) {
      this->a[i] = this->a[i << 1] * this->a[(i << 1) | 1];
    }
  }

  std::size_t size() const { return n; }

  const M& get(std::size_t i) const {
    assert(i < n);
    return a[sz + i];
  }

  template <typename T>
    requires std::constructible_from<M, const T&>
  void set(std::size_t i, const T& v) {
    assert(i < n);
    i += sz;
    a[i] = M{v};
    while (i > 1) {
      i >>= 1;
      a[i] = a[i << 1] * a[(i << 1) | 1];
    }
  }

  M fold() const { return a[1]; }

  M fold(std::size_t l, std::size_t r) const {
    assert(l <= r);
    assert(r <= n);
    M lcum = M::identity(), rcum = M::identity();
    l += sz;
    r += sz;
    while (l < r) {
      if (l & 1) {
        lcum = lcum * a[l];
        ++l;
      }
      if (r & 1) {
        --r;
        rcum = a[r] * rcum;
      }
      l >>= 1;
      r >>= 1;
    }
    return lcum * rcum;
  }

  template <typename F>
  std::size_t max_right(std::size_t l, F f) const
    requires std::predicate<F&, M>
  {
    assert(l <= n);
    assert(f(M::identity()));

    if (l == n) return n;

    auto i = sz + l;
    M cum = M::identity();
    while (true) {
      while ((i & 1) == 0) i >>= 1;

      if (!f(cum * a[i])) {
        while (i < sz) {
          i <<= 1;
          if (f(cum * a[i])) {
            cum = cum * a[i];
            ++i;
          }
        }
        return i - sz;
      }

      cum = cum * a[i];

      if (std::has_single_bit(i + 1)) break;
      ++i;
    }
    return n;
  }

  template <typename F>
  std::size_t min_left(std::size_t r, F f) const
    requires std::predicate<F&, M>
  {
    assert(r <= n);
    assert(f(M::identity()));

    if (r == 0) return 0;

    auto i = sz + r - 1;
    M cum = M::identity();
    while (true) {
      while (i > 1 && (i & 1)) i >>= 1;

      if (!f(a[i] * cum)) {
        while (i < sz) {
          i = (i << 1) | 1;
          if (f(a[i] * cum)) {
            cum = a[i] * cum;
            --i;
          }
        }
        return i + 1 - sz;
      }

      cum = a[i] * cum;

      if (std::has_single_bit(i)) break;
      --i;
    }
    return 0;
  }

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
  std::size_t n, sz;
  std::vector<M> a;
};
