#pragma once

#include <cassert>
#include <cstddef>
#include <numeric>
#include <utility>
#include <vector>

class Permutation {
 public:
  explicit Permutation(std::size_t n): p(n), sgn{1} {
    std::ranges::iota(p, 0);
    q = p;
  }
  explicit Permutation(std::vector<int> p)
      : p(std::move(p)), q(this->p.size()), sgn{0} {
    int n = this->p.size();
    for (int i = 0; i < n; ++i) q[this->p[i]] = i;
  }

  std::size_t size() const { return p.size(); }

  int operator[](std::size_t i) const {
    assert(i < p.size());
    return p[i];
  }
  int preimage(std::size_t i) const {
    assert(i < p.size());
    return q[i];
  }

  const std::vector<int>& unwrap() const { return p; }
  const std::vector<int>& inverse_unwrap() const { return q; }

  Permutation inverse() const {
    Permutation r{q};
    r.sgn = sgn;
    return r;
  }

  friend Permutation operator*(const Permutation& p, const Permutation& q) {
    assert(p.size() == q.size());
    int n = p.size();
    std::vector<int> inner(n);
    for (int i = 0; i < n; ++i) inner[i] = p[q[i]];
    Permutation r{std::move(inner)};
    r.sgn = p.sgn * q.sgn;
    return r;
  }

  int sign() const {
    if (sgn == 0) {
      int n = p.size(), ncc = 0;
      std::vector<bool> vis(n);
      for (int i = 0; i < n; ++i) {
        if (vis[i]) continue;
        int u = i;
        do {
          vis[u] = true;
          u = p[u];
        } while (u != i);
        ++ncc;
      }
      sgn = (n - ncc) % 2 == 0 ? 1 : -1;
    }
    return sgn;
  }

  void invert() { std::swap(p, q); }

  void swap(std::size_t i, std::size_t j) {
    assert(i < p.size());
    assert(j < p.size());
    if (i == j) return;
    std::swap(q[p[i]], q[p[j]]);
    std::swap(p[i], p[j]);
    sgn *= -1;
  }

  std::vector<int> cycle(std::size_t i) const {
    assert(i < p.size());
    std::vector<int> C;
    auto u = i;
    do {
      C.emplace_back(u);
      u = p[u];
    } while (u != i);
    return C;
  }

  std::vector<std::vector<int>> cycles() const {
    std::vector<std::vector<int>> Cs;
    int n = p.size();
    std::vector<bool> vis(n);
    for (int i = 0; i < n; ++i) {
      if (vis[i]) continue;
      auto C = cycle(i);
      for (int u: C) vis[u] = true;
      Cs.emplace_back(std::move(C));
    }
    return Cs;
  }

 private:
  std::vector<int> p, q;
  mutable int sgn;
};
