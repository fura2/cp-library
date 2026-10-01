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
        x(2 * sz, X::identity()),
        f(2 * sz, F::identity()) {}

  template <typename T = X>
    requires std::constructible_from<X, const T&>
  explicit LazySegmentTree(const std::vector<T>& a)
      : n(a.size()),
        sz(std::bit_ceil<unsigned int>(n)),
        x(2 * sz, X::identity()),
        f(2 * sz, F::identity()) {
    for (int i = 0; i < n; ++i) {
      x[sz + i] = X{a[i]};
    }
    for (int i = sz - 1; i > 0; --i) {
      x[i] = x[i << 1] * x[(i << 1) | 1];
    }
  }

  int size() const { return n; }

  const X& get(int i) const {
    assert(0 <= i && i < n);
    // TODO
  }

  template <typename T = X>
    requires std::constructible_from<X, const T&>
  void set(int i, const T& v) {
    assert(0 <= i && i < n);
    // TODO
  }

  const X& fold() const {
    // TODO
    return x[1];
  }

  X fold(int l, int r) const {
    assert(0 <= l && l <= r && r <= n);
    return fold(1, 0, sz, l, r);
  }

  template <typename T = F>
    requires std::constructible_from<F, const T&>
  void apply(int l, int r, const T& v) {
    assert(0 <= l && l <= r && r <= n);
    apply(1, 0, sz, l, r, v);
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
  int n, sz;
  mutable std::vector<X> x;
  mutable std::vector<F> f;

  X fold(int u, int a, int b, int l, int r) const {
    propagate(u);
    if (b <= l || r <= a) return X::identity();
    if (l <= a && b <= r) return x[u];
    int c = (a + b) / 2;
    return fold(2 * u, a, c, l, r) * fold(2 * u + 1, c, b, l, r);
  }

  // 不変条件: この関数の実行後はつねに f[u] == id
  template <typename T = F>
    requires std::constructible_from<F, const T&>
  void apply(int u, int a, int b, int l, int r, const T& v) {
    propagate(u);
    if (b <= l || r <= a) return;
    if (l <= a && b <= r) {
      f[u] = F{v};
      propagate(u);
      return;
    }
    int c = (a + b) / 2;
    apply(2 * u, a, c, l, r, v);
    apply(2 * u + 1, c, b, l, r, v);
    x[u] = x[2 * u] * x[2 * u + 1];
  }

  void propagate(int u) const {
    // if (f[u] == F::identity()) return;
    if (f[u].unwrap() == F::identity().unwrap()) return;  // TODO: あとで直す
    x[u] = A::act(x[u], f[u]);
    if (u < sz) {
      f[2 * u] = f[2 * u] * f[u];
      f[2 * u + 1] = f[2 * u + 1] * f[u];
    }
    f[u] = F::identity();
  }
};
