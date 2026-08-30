#pragma once

#include <algorithm>
#include <cassert>
#include <concepts>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <utility>
#include <vector>

#include "mint/mint.hpp"

template <typename R>
class FormalPowerSeries {
 public:
  explicit FormalPowerSeries(int n = inf): n{n} { assert(n >= 0); }

  template <typename T>
    requires std::constructible_from<R, const T&>
  explicit FormalPowerSeries(const std::vector<T>& a, int n = inf): n{n} {
    assert(n >= 0);
    auto k = std::min<int>(a.size(), n);
    this->a.reserve(k);
    for (int i = 0; i < k; ++i) {
      this->a.emplace_back(a[i]);
    }
  }

  explicit FormalPowerSeries(std::vector<R>&& a, int n = inf)
      : n{n}, a{std::move(a)} {
    assert(n >= 0);
    if (std::ssize(this->a) > n) this->a.resize(n);
  }

  template <typename T>
    requires std::constructible_from<R, const T&>
  FormalPowerSeries(std::initializer_list<T> a, int n = inf): n{n} {
    assert(n >= 0);
    auto k = std::min<int>(a.size(), n);
    this->a.reserve(k);
    int i = 0;
    for (const auto& x: a) {
      if (i >= k) break;
      this->a.emplace_back(x);
      ++i;
    }
  }

  FormalPowerSeries& operator+=(const FormalPowerSeries& f) {
    truncate(f.n);
    int k = std::min<int>(n, f.a.size());
    if (std::ssize(a) < k) a.resize(k);
    for (int i = 0; i < k; ++i) a[i] += f.a[i];
    return *this;
  }
  FormalPowerSeries& operator-=(const FormalPowerSeries& f) {
    truncate(f.n);
    int k = std::min<int>(n, f.a.size());
    if (std::ssize(a) < k) a.resize(k);
    for (int i = 0; i < k; ++i) a[i] -= f.a[i];
    return *this;
  }

  friend FormalPowerSeries operator+(FormalPowerSeries f,
                                     const FormalPowerSeries& g) {
    f += g;
    return f;
  }
  friend FormalPowerSeries operator-(FormalPowerSeries f,
                                     const FormalPowerSeries& g) {
    f -= g;
    return f;
  }

  R operator[](int i) const {
    assert(0 <= i && i < n);
    return i < std::ssize(a) ? a[i] : R{};
  }
  R& operator[](int i) {
    assert(0 <= i && i < n);
    if (std::ssize(a) <= i) a.resize(i + 1);
    return a[i];
  }

 private:
  static constexpr int inf = std::numeric_limits<int>::max();

  int n;
  std::vector<R> a;

  void truncate(int m) {
    assert(m >= 0);
    if (n > m) {
      n = m;
      if (std::ssize(a) > n) a.resize(n);
    }
  }
};

using fps = FormalPowerSeries<mint>;
