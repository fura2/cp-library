#pragma once

#include <cassert>

#include "mint/mod_int.hpp"

template <typename T>
  requires is_mod_int<T> && (T::mod() >= 3)
class BinomialRangeSum {
 public:
  explicit BinomialRangeSum(int n, int l, int r): n{n}, l{l}, r{r}, s{0} {
    assert(n >= 0);
    assert(0 <= l && l <= r);
    for (int i = l; i < r; ++i) {
      s += choose<T>(n, i);
    }
  }

  const T& sum(int n, int l, int r) {
    assert(n >= 0);
    assert(0 <= l && l <= r);
    while (this->n < n) {
      s = s + s + choose<T>(this->n, this->l - 1) -
          choose<T>(this->n, this->r - 1);
      ++this->n;
    }
    while (this->n > n) {
      s = (s - choose<T>(this->n - 1, this->l - 1) +
           choose<T>(this->n - 1, this->r - 1)) *
          inverse<T>(2);
      --this->n;
    }
    while (this->l < l) {
      s -= choose<T>(n, this->l);
      ++this->l;
    }
    while (this->l > l) {
      s += choose<T>(n, this->l - 1);
      --this->l;
    }
    while (this->r < r) {
      s += choose<T>(n, this->r);
      ++this->r;
    }
    while (this->r > r) {
      s -= choose<T>(n, this->r - 1);
      --this->r;
    }
    return s;
  }

 private:
  int n, l, r;
  T s;
};
