#pragma once

#include <cassert>
#include <cstddef>
#include <numeric>
#include <utility>
#include <vector>

class Permutation {
 public:
  explicit Permutation(std::size_t n): p(n) {
    std::ranges::iota(p, 0);
    q = p;
  }
  explicit Permutation(std::vector<int> p): p(std::move(p)), q(this->p.size()) {
    int n = this->p.size();
    for (int i = 0; i < n; ++i) q[this->p[i]] = i;
  }

  std::size_t size() const { return p.size(); }

  friend Permutation operator*(const Permutation& p, const Permutation& q) {
    assert(p.size() == q.size());
    int n = p.size();
    std::vector<int> r(n);
    for (int i = 0; i < n; ++i) r[i] = p[q[i]];
    return Permutation{r};
  }

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

  Permutation inverse() const { return Permutation{q}; }

  void invert() { std::swap(p, q); }

  void swap(std::size_t i, std::size_t j) {
    assert(i < p.size());
    assert(j < p.size());
    std::swap(q[p[i]], q[p[j]]);
    std::swap(p[i], p[j]);
  }

 private:
  std::vector<int> p, q;
};
