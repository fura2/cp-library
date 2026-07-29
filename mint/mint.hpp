#pragma once

#include <cassert>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include "template/io.hpp"

class mint {
  static const int MOD = 998244353;

 public:
  mint(): x{0} {}
  mint(long long x) {
    this->x = x % MOD;
    if (this->x < 0) this->x += MOD;
  }

  mint& operator+=(const mint& m) {
    x += m.x;
    if (x >= MOD) x -= MOD;
    return *this;
  }
  mint& operator-=(const mint& m) {
    x -= m.x;
    if (x < 0) x += MOD;
    return *this;
  }
  mint& operator*=(const mint& m) {
    x = static_cast<long long>(x) * m.x % MOD;
    return *this;
  }
  mint& operator/=(const mint& m) { return *this *= m.inverse(); }
  friend mint operator+(const mint& m, const mint& n) { return mint{m} += n; }
  friend mint operator-(const mint& m, const mint& n) { return mint{m} -= n; }
  friend mint operator*(const mint& m, const mint& n) { return mint{m} *= n; }
  friend mint operator/(const mint& m, const mint& n) { return mint{m} /= n; }
  mint operator-() const { return mint{-x}; }

  friend mint operator+(long long x, const mint& m) { return mint{x} + m; }
  friend mint operator-(long long x, const mint& m) { return mint{x} - m; }
  friend mint operator*(long long x, const mint& m) { return mint{x} * m; }
  friend mint operator/(long long x, const mint& m) { return mint{x} / m; }

  mint inverse() const {
    assert(x != 0);
    int a = x, b = MOD, u = 1, v = 0;
    while (b > 0) {
      int t = a / b;
      a -= t * b;
      std::swap(a, b);
      u -= t * v;
      std::swap(u, v);
    }
    return u;
  }

  int unwrap() const { return x; }

  friend void output(const mint& m) { output(m.x); }

  friend std::string pretty(const mint& m) { return std::to_string(m.x); }

 private:
  int x;
};

template <>
struct Input<mint> {
  static mint read() { return mint{input<long long>()}; }
};

inline mint pow(mint m, long long k) {
  mint res = 1;
  for (; k > 0; k >>= 1, m *= m)
    if (k & 1) res *= m;
  return res;
}

inline mint factorial(int n) {
  assert(n >= 0);
  static std::vector<mint> memo = {1};
  if (std::ssize(memo) <= n) {
    int k = memo.size();
    memo.resize(n + 1);
    for (; k <= n; ++k) memo[k] = memo[k - 1] * k;
  }
  return memo[n];
}

inline mint factorial_inverse(int n) {
  assert(n >= 0);
  static std::vector<mint> memo = {1};
  if (std::ssize(memo) <= n) {
    int k = memo.size();
    memo.resize(n + 1);
    memo[n] = factorial(n).inverse();
    for (int i = n; i > k; i--) memo[i - 1] = memo[i] * i;
  }
  return memo[n];
}

inline mint choose(int n, int k, int type = 0) {
  assert(n >= 0);
  if (k < 0 || n < k) return 0;
  if (type == 0) {
    return factorial(n) * factorial_inverse(k) * factorial_inverse(n - k);
  }
  else {
    if (k > n - k) k = n - k;
    mint res = factorial_inverse(k);
    for (int i = 0; i < k; ++i) res *= n - i;
    return res;
  }
}

inline mint multichoose(int n, int k, int type = 0) {
  assert(n >= 0);
  assert(k >= 0);
  if (n == 0 && k == 0) return 1;
  return choose(n + k - 1, k, type);
}
