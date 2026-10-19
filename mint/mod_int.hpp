#pragma once

#include <cassert>
#include <iterator>
#include <limits>
#include <string>
#include <utility>
#include <vector>

#include "algebra/collection/id.hpp"
#include "template/io.hpp"

template <int Mod>
class ModInt {
  static_assert(0 < Mod && Mod <= std::numeric_limits<int>::max() / 2);

 public:
  ModInt(): x{0} {}
  ModInt(long long x) {
    this->x = x % Mod;
    if (this->x < 0) this->x += Mod;
  }

  ModInt& operator+=(const ModInt& m) {
    x += m.x;
    if (x >= Mod) x -= Mod;
    return *this;
  }
  ModInt& operator-=(const ModInt& m) {
    x -= m.x;
    if (x < 0) x += Mod;
    return *this;
  }
  ModInt& operator*=(const ModInt& m) {
    x = static_cast<long long>(x) * m.x % Mod;
    return *this;
  }
  ModInt& operator/=(const ModInt& m) { return *this *= m.inverse(); }
  friend ModInt operator+(ModInt m, const ModInt& n) {
    m += n;
    return m;
  }
  friend ModInt operator-(ModInt m, const ModInt& n) {
    m -= n;
    return m;
  }
  friend ModInt operator*(ModInt m, const ModInt& n) {
    m *= n;
    return m;
  }
  friend ModInt operator/(ModInt m, const ModInt& n) {
    m /= n;
    return m;
  }
  ModInt operator-() const { return ModInt{-x}; }

  friend ModInt operator+(long long x, const ModInt& m) {
    return ModInt{x} + m;
  }
  friend ModInt operator-(long long x, const ModInt& m) {
    return ModInt{x} - m;
  }
  friend ModInt operator*(long long x, const ModInt& m) {
    return ModInt{x} * m;
  }
  friend ModInt operator/(long long x, const ModInt& m) {
    return ModInt{x} / m;
  }

  ModInt inverse() const {
    assert(x != 0);
    int a = x, b = Mod, u = 1, v = 0;
    while (b > 0) {
      int t = a / b;
      a -= t * b;
      std::swap(a, b);
      u -= t * v;
      std::swap(u, v);
    }
    assert(a == 1);
    return u;
  }

  int unwrap() const { return x; }

  static constexpr int mod() { return Mod; }

  friend std::string pretty(const ModInt& m) { return std::to_string(m.x); }

 private:
  int x;
};

template <int Mod>
struct Input<ModInt<Mod>> {
  static ModInt<Mod> read() { return ModInt<Mod>{Input<long long>::read()}; }
};

template <int Mod>
struct Output<ModInt<Mod>> {
  static void write(const ModInt<Mod>& m) { Output<int>::write(m.unwrap()); }
};

template <typename T>
inline constexpr bool is_mod_int = false;
template <int Mod>
inline constexpr bool is_mod_int<ModInt<Mod>> = true;

template <typename T>
  requires is_mod_int<T>
inline T pow(T m, long long k) {
  T res = 1;
  for (; k > 0; k >>= 1, m *= m)
    if (k & 1) res *= m;
  return res;
}

template <typename T>
  requires is_mod_int<T>
inline T inverse(int n) {
  assert(0 < n && n < T::mod());
  static std::vector<T> memo = {0, 1};
  if (std::ssize(memo) <= n) {
    int k = memo.size();
    memo.resize(n + 1);
    for (; k <= n; ++k) memo[k] = -memo[T::mod() % k] * (T::mod() / k);
  }
  return memo[n];
}

template <typename T>
  requires is_mod_int<T>
inline T factorial(int n) {
  assert(n >= 0);
  static std::vector<T> memo = {1};
  if (std::ssize(memo) <= n) {
    int k = memo.size();
    memo.resize(n + 1);
    for (; k <= n; ++k) memo[k] = memo[k - 1] * k;
  }
  return memo[n];
}

template <typename T>
  requires is_mod_int<T>
inline T factorial_inverse(int n) {
  assert(n >= 0);
  static std::vector<T> memo = {1};
  if (std::ssize(memo) <= n) {
    int k = memo.size();
    memo.resize(n + 1);
    inverse<T>(n);
    for (; k <= n; ++k) memo[k] = memo[k - 1] * inverse<T>(k);
  }
  return memo[n];
}

template <typename T>
  requires is_mod_int<T>
inline T choose(int n, int k, int type = 0) {
  assert(n >= 0);
  if (k < 0 || n < k) return 0;
  if (type == 0) {
    return factorial<T>(n) * factorial_inverse<T>(k) *
           factorial_inverse<T>(n - k);
  }
  else {
    if (k > n - k) k = n - k;
    T res = factorial_inverse<T>(k);
    for (int i = 0; i < k; ++i) res *= n - i;
    return res;
  }
}

template <typename T>
  requires is_mod_int<T>
inline T multichoose(int n, int k, int type = 0) {
  assert(n >= 0);
  assert(k >= 0);
  if (n == 0 && k == 0) return 1;
  return choose<T>(n + k - 1, k, type);
}

template <typename T>
  requires is_mod_int<T>
inline T multinomial(const std::vector<int>& a) {
  T res = 1;
  int n = a.size(), s = 0;
  for (int i = 0; i < n; ++i) {
    assert(a[i] >= 0);
    s += a[i];
    res *= choose<T>(s, a[i]);
  }
  return res;
}

template <int Mod>
inline constexpr auto id_zero<ModInt<Mod>> = []() { return ModInt<Mod>{0}; };

template <int Mod>
inline constexpr auto id_one<ModInt<Mod>> = []() { return ModInt<Mod>{1}; };
