#pragma once

#include <bit>
#include <cassert>
#include <concepts>
#include <string>
#include <vector>

#include "algebra/monoid_action.hpp"

template <MonoidAction A>
class LazySegmentTree {
  using M = A::value_monoid;
  using F = A::action_monoid;

 public:
  explicit LazySegmentTree(int n)
      : n{n},
        sz(std::bit_ceil<unsigned int>(n)),
        m(2 * sz, M::identity()),
        f(2 * sz, F::identity()) {}

  template <typename T = M>
    requires std::constructible_from<M, const T&>
  explicit LazySegmentTree(const std::vector<T>& a)
      : n(a.size()),
        sz(std::bit_ceil<unsigned int>(n)),
        m(2 * sz, M::identity()),
        f(2 * sz, F::identity()) {
    for (int i = 0; i < n; ++i) {
      m[sz + i] = M{a[i]};
    }
    for (int i = sz - 1; i > 0; --i) {
      m[i] = m[i << 1] * m[(i << 1) | 1];
    }
  }

  int size() const { return n; }

  const M& get(int i) const {
    assert(0 <= i && i < n);
    // TODO
  }

  template <typename T = M>
    requires std::constructible_from<M, const T&>
  void set(int i, const T& v) {
    assert(0 <= i && i < n);
    // TODO
  }

  const M& fold() const {
    // TODO
    return m[1];
  }

  M fold(int l, int r) const {
    assert(0 <= l && l <= r && r <= n);
    return fold(1, 0, sz, l, r);
  }

  void apply(int l, int r, const F& f) {
    assert(0 <= l && l <= r && r <= n);
    apply(1, 0, sz, l, r, f);
  }

  template <typename G>
    requires std::predicate<G&, M>
  int max_right(int l, G g) const {
    assert(0 <= l && l <= n);
    assert(g(M::identity()));
    // TODO
  }

  template <typename G>
    requires std::predicate<G&, M>
  int min_left(int r, G g) const {
    assert(0 <= r && r <= n);
    assert(g(M::identity()));
    // TODO
  }

  friend std::string pretty(const LazySegmentTree& S) {
    if constexpr (requires(const M& x) {
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
  mutable std::vector<M> m;
  mutable std::vector<F> f;

  M fold(int u, int a, int b, int l, int r) const {
    propagate(u);
    if (b <= l || r <= a) return M::identity();
    if (l <= a && b <= r) return m[u];
    int c = (a + b) / 2;
    return fold(2 * u, a, c, l, r) * fold(2 * u + 1, c, b, l, r);
  }

  // 不変条件: この関数の実行後はつねに f[u] == id
  void apply(int u, int a, int b, int l, int r, const F& f_) {
    propagate(u);
    if (b <= l || r <= a) return;
    if (l <= a && b <= r) {
      f[u] = f_;
      propagate(u);
      return;
    }
    int c = (a + b) / 2;
    apply(2 * u, a, c, l, r, f_);
    apply(2 * u + 1, c, b, l, r, f_);
    m[u] = m[2 * u] * m[2 * u + 1];
  }

  void propagate(int u) const {
    // if (f[u] == F::identity()) return;
    if (f[u].unwrap() == F::identity().unwrap()) return;  // TODO: あとで直す
    m[u] = A::act(m[u], f[u]);
    if (u < sz) {
      f[2 * u] = f[2 * u] * f[u];
      f[2 * u + 1] = f[2 * u + 1] * f[u];
    }
    f[u] = F::identity();
  }
};
