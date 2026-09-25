#pragma once

#include <bit>
#include <vector>

#include "algebra/monoid_action.hpp"

template <MonoidAction A>
class LazySegmentTree {
  using M = A::value_monoid;
  using F = A::action_monoid;

 public:
  LazySegmentTree(int n): e{M::identity()}, id{F::identity()} {
    this->n = std::bit_ceil<unsigned int>(n);
    m.assign(2 * this->n, e);
    f.assign(2 * this->n, id);
  }

  LazySegmentTree(const std::vector<M>& v)
      : e{M::identity()}, id{F::identity()} {
    n = std::bit_ceil(v.size());
    m.assign(2 * n, e);
    f.assign(2 * n, id);
    for (int i = 0; i < v.size(); ++i) {
      m[n + i] = v[i];
    }
    for (int i = n - 1; i >= 1; i--) {
      m[i] = m[i << 1] * m[(i << 1) | 1];
    }
  }

  void apply(int l, int r, const F& f) { apply(1, 0, n, l, r, f); }

  const M& fold() const { return m[1]; }

  M fold(int l, int r) { return fold(1, 0, n, l, r); }

 private:
  int n;
  std::vector<M> m;
  std::vector<F> f;
  const M e;
  const F id;

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

  M fold(int u, int a, int b, int l, int r) {
    propagate(u);
    if (b <= l || r <= a) return e;
    if (l <= a && b <= r) return m[u];
    int c = (a + b) / 2;
    return fold(2 * u, a, c, l, r) * fold(2 * u + 1, c, b, l, r);
  }

  void propagate(int u) {
    // if (f[u] == id) return;
    if (f[u].unwrap() == id.unwrap()) return;  // TODO: あとで直す
    m[u] = A::act(m[u], f[u]);
    if (u < n) {
      f[2 * u] = f[2 * u] * f[u];
      f[2 * u + 1] = f[2 * u + 1] * f[u];
    }
    f[u] = id;
  }
};
