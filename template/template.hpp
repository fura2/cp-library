#pragma once

#include <bits/stdc++.h>

#ifdef LOCAL
#include <template/debug.hpp>
#else
#define show(...) ((void)0)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#endif

#include "template/constant.hpp"
#include "template/io.hpp"

#define impl_overload4(a, b, c, d, e, ...) e

// clang-format off
#define impl_rep4(i, a, b, c) for (int i = (a); i < (b); i += (c))
#define impl_rep3(i, a, b) impl_rep4(i, a, b, 1)
#define impl_rep2(i, n) impl_rep3(i, 0, n)
#define impl_rep1(n) for (int _ = 0; _ < (n); ++_)
#define rep(...) impl_overload4(__VA_ARGS__, impl_rep4, impl_rep3, impl_rep2, impl_rep1)(__VA_ARGS__)

#define impl_rrep4(i, a, b, c) for (int i = (b) - 1; i >= (a); i -= (c))
#define impl_rrep3(i, a, b) impl_rrep4(i, a, b, 1)
#define impl_rrep2(i, n) impl_rrep3(i, 0, n)
#define rrep(...) impl_overload4(__VA_ARGS__, impl_rrep4, impl_rrep3, impl_rrep2)(__VA_ARGS__)
// clang-format on

template <typename T>
constexpr int bit(T x, unsigned int k) {
  return (x >> k) & 1;
}

template <typename T>
constexpr bool chmax(T& a, const T& b) {
  return a < b ? a = b, true : false;
}
template <typename T>
constexpr bool chmin(T& a, const T& b) {
  return b < a ? a = b, true : false;
}

inline void yesno(bool b) { std::cout << (b ? "Yes" : "No") << "\n"; }
inline void yes() { yesno(true); }
inline void no() { yesno(false); }

inline const auto io_setup = [] {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(10);
  return 0;
}();

using lint = long long;

using namespace std;
