#pragma once

#include <bit>
#include <cassert>
#include <concepts>
#include <string>
#include <vector>

#include "algebra/monoid_action.hpp"

template <MonoidActionOnMonoid A>
class LazySegmentTree {
  using X = A::value_type;
  using F = A::action_type;

 public:
  explicit LazySegmentTree(int n)
      : n{n},
        sz(std::bit_ceil<unsigned int>(n)),
        h{std::bit_width<unsigned int>(sz)},
        x(2 * sz, X::identity()),
        f(sz, F::identity()) {}

  template <typename T = X>
    requires std::constructible_from<X, const T&>
  explicit LazySegmentTree(const std::vector<T>& a)
      : n(a.size()),
        sz(std::bit_ceil<unsigned int>(n)),
        h{std::bit_width<unsigned int>(sz)},
        x(2 * sz, X::identity()),
        f(sz, F::identity()) {
    for (int i = 0; i < n; ++i) {
      x[sz + i] = X{a[i]};
    }
    for (int u = sz - 1; u > 0; --u) {
      x[u] = x[u << 1] * x[(u << 1) | 1];
    }
  }

  int size() const { return n; }

  const X& get(int i) const {
    assert(0 <= i && i < n);
    int u = sz + i;
    push(u);
    return x[u];
  }

  template <typename T = X>
    requires std::constructible_from<X, const T&>
  void set(int i, const T& v) {
    assert(0 <= i && i < n);
    int u = sz + i;
    push(u);
    x[u] = X{v};
    fix(u);
  }

  const X& fold() const { return x[1]; }

  X fold(int l, int r) const {
    assert(0 <= l && l <= r && r <= n);
    if (l == r) return X::identity();
    l += sz;
    r += sz;

    push(l);
    push(r - 1);

    X lcum = X::identity(), rcum = X::identity();
    while (l < r) {
      if (l & 1) {
        lcum = lcum * x[l];
        ++l;
      }
      if (r & 1) {
        --r;
        rcum = x[r] * rcum;
      }
      l >>= 1;
      r >>= 1;
    }

    return lcum * rcum;
  }

  template <typename T = F>
    requires std::constructible_from<F, const T&>
  void apply(int l, int r, const T& v) {
    assert(0 <= l && l <= r && r <= n);
    if (l == r) return;
    l += sz;
    r += sz;

    push(l);
    push(r - 1);

    int l2 = l, r2 = r;
    while (l < r) {
      if (l & 1) {
        calc(l, F{v});
        ++l;
      }
      if (r & 1) {
        --r;
        calc(r, F{v});
      }
      l >>= 1;
      r >>= 1;
    }
    l = l2;
    r = r2;

    fix(l);
    fix(r - 1);
  }

  template <typename G>
    requires std::predicate<G&, X>
  int max_right(int l, G g) const {
    assert(0 <= l && l <= n);
    assert(g(X::identity()));
    // TODO
  }

  template <typename G>
    requires std::predicate<G&, X>
  int min_left(int r, G g) const {
    assert(0 <= r && r <= n);
    assert(g(X::identity()));
    // TODO
  }

  friend std::string pretty(const LazySegmentTree& S) {
    if constexpr (requires(const X& x) {
                    { pretty(x) } -> std::same_as<std::string>;
                  }) {
      std::string s = "[";
      for (int i = 0; i < S.size(); ++i) {
        s += (i == 0 ? "" : ", ") + pretty(S.get(i));
      }
      s += "]";
      return s;
    }
    return "[" + std::to_string(S.size()) + " element(s)]";
  }

 private:
  int n, sz, h;
  mutable std::vector<X> x;
  mutable std::vector<F> f;

  void fix(int u) {
    while (u > 1) {
      u >>= 1;
      x[u] = A::act(x[u << 1] * x[(u << 1) | 1], f[u]);
    }
  }

  void push(int u) const {
    for (int k = h - 1; k > 0; --k) {
      int v = u >> k;
      calc(v << 1, f[v]);
      calc((v << 1) | 1, f[v]);
      f[v] = F::identity();
    }
  }

  void calc(int u, const F& g) const {
    x[u] = A::act(x[u], g);
    if (u < sz) {
      f[u] = f[u] * g;
    }
  }
};
