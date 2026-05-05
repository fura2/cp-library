#pragma once

#include <array>
#include <cstddef>
#include <deque>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "template/constant.hpp"

#define impl_overload7(a, b, c, d, e, f, g, h, ...) h

// clang-format off
#define impl_show1(a) { std::clog << #a << " = " << pretty(a) << "\n"; }
#define impl_show2(a, ...) { std::clog << #a << " = " << pretty(a) << ", "; impl_show1(__VA_ARGS__); }
#define impl_show3(a, ...) { std::clog << #a << " = " << pretty(a) << ", "; impl_show2(__VA_ARGS__); }
#define impl_show4(a, ...) { std::clog << #a << " = " << pretty(a) << ", "; impl_show3(__VA_ARGS__); }
#define impl_show5(a, ...) { std::clog << #a << " = " << pretty(a) << ", "; impl_show4(__VA_ARGS__); }
#define impl_show6(a, ...) { std::clog << #a << " = " << pretty(a) << ", "; impl_show5(__VA_ARGS__); }
#define impl_show7(a, ...) { std::clog << #a << " = " << pretty(a) << ", "; impl_show6(__VA_ARGS__); }
#define show(...) \
  do { \
    std::clog << "\033[33m"; \
    impl_overload7(__VA_ARGS__, impl_show7, impl_show6, impl_show5, impl_show4, impl_show3, impl_show2, impl_show1)(__VA_ARGS__) \
    std::clog << "\033[0m" << std::flush; \
  } while (false)
// clang-format on

// declarations for containers

template <typename T>
std::string pretty(const std::optional<T>& v);

template <typename T1, typename T2>
std::string pretty(const std::pair<T1, T2>& p);

template <typename... Ts>
std::string pretty(const std::tuple<Ts...>& t);

template <typename T, std::size_t N>
std::string pretty(const T (&a)[N]);

template <typename T, std::size_t N>
std::string pretty(const std::array<T, N>& a);

template <typename T, typename Allocator>
std::string pretty(const std::vector<T, Allocator>& v);

template <typename T, typename Allocator>
std::string pretty(const std::deque<T, Allocator>& d);

template <typename Key, typename Compare, typename Allocator>
std::string pretty(const std::set<Key, Compare, Allocator>& s);

template <typename Key, typename Compare, typename Allocator>
std::string pretty(const std::multiset<Key, Compare, Allocator>& s);

template <typename Key, typename Hash, typename Pred, typename Allocator>
std::string pretty(const std::unordered_set<Key, Hash, Pred, Allocator>& s);

template <typename Key, typename Hash, typename Pred, typename Allocator>
std::string pretty(
    const std::unordered_multiset<Key, Hash, Pred, Allocator>& s);

template <typename Key, typename T, typename Compare, typename Allocator>
std::string pretty(const std::map<Key, T, Compare, Allocator>& m);

template <typename Key, typename T, typename Compare, typename Allocator>
std::string pretty(const std::multimap<Key, T, Compare, Allocator>& m);

template <typename Key,
          typename T,
          typename Hash,
          typename Pred,
          typename Allocator>
std::string pretty(const std::unordered_map<Key, T, Hash, Pred, Allocator>& m);

template <typename Key,
          typename T,
          typename Hash,
          typename Pred,
          typename Allocator>
std::string pretty(
    const std::unordered_multimap<Key, T, Hash, Pred, Allocator>& m);

// implementations for non-container types

inline std::string pretty(int x) {
  return x == INF ? "∞" : x == -INF ? "-∞" : std::to_string(x);
}

inline std::string pretty(unsigned int x) { return std::to_string(x); }

inline std::string pretty(long x) { return std::to_string(x); }

inline std::string pretty(unsigned long x) { return std::to_string(x); }

inline std::string pretty(long long x) {
  return x == LINF ? "∞" : x == -LINF ? "-∞" : std::to_string(x);
}

inline std::string pretty(unsigned long long x) { return std::to_string(x); }

inline std::string pretty(float x) { return std::to_string(x); }

inline std::string pretty(double x) { return std::to_string(x); }

inline std::string pretty(long double x) { return std::to_string(x); }

inline std::string pretty(bool b) { return b ? "⊤" : "⊥"; }

inline std::string pretty(char c) {
  if (c == '\0') return "'\\0'";
  if (c == '\t') return "'\\t'";
  if (c == '\n') return "'\\n'";
  if (c == '\r') return "'\\r'";
  return "'" + std::string{c} + "'";
}

inline std::string pretty(const char* s) {
  return s ? "\"" + std::string(s) + "\"" : "null";
}

inline std::string pretty(const std::string& s) { return "\"" + s + "\""; }

// implementations for containers

template <typename T>
std::string pretty(const std::optional<T>& v) {
  return v.has_value() ? pretty(v.value()) : "*";
}

template <typename T1, typename T2>
std::string pretty(const std::pair<T1, T2>& p) {
  return "(" + pretty(p.first) + ", " + pretty(p.second) + ")";
}

template <typename... Ts>
std::string pretty(const std::tuple<Ts...>& t) {
  std::string s = "(";
  [&]<std::size_t... I>(std::index_sequence<I...>) {
    ((s += (I == 0 ? "" : ", ") + pretty(std::get<I>(t))), ...);
  }(std::index_sequence_for<Ts...>{});
  s += ")";
  return s;
}

template <typename T, std::size_t N>
std::string pretty(const T (&a)[N]) {
  std::string s = "[";
  for (int i = 0; const auto& e: a) {
    s += (i == 0 ? "" : ", ") + pretty(e);
    ++i;
  }
  s += "]";
  return s;
}

template <typename T, std::size_t N>
std::string pretty(const std::array<T, N>& a) {
  std::string s = "[";
  for (int i = 0; const auto& e: a) {
    s += (i == 0 ? "" : ", ") + pretty(e);
    ++i;
  }
  s += "]";
  return s;
}

template <typename T, typename Allocator>
std::string pretty(const std::vector<T, Allocator>& v) {
  std::string s = "[";
  for (int i = 0; const auto& e: v) {
    s += (i == 0 ? "" : ", ") + pretty(e);
    ++i;
  }
  s += "]";
  return s;
}

template <typename T, typename Allocator>
std::string pretty(const std::deque<T, Allocator>& d) {
  std::string s = "[";
  for (int i = 0; const auto& e: d) {
    s += (i == 0 ? "" : ", ") + pretty(e);
    ++i;
  }
  s += "]";
  return s;
}

template <typename Key, typename Compare, typename Allocator>
std::string pretty(const std::set<Key, Compare, Allocator>& s) {
  std::string t = "{";
  for (int i = 0; const auto& e: s) {
    t += (i == 0 ? "" : ", ") + pretty(e);
    ++i;
  }
  t += "}";
  return t;
}

template <typename Key, typename Compare, typename Allocator>
std::string pretty(const std::multiset<Key, Compare, Allocator>& s) {
  std::string t = "{";
  for (int i = 0; const auto& e: s) {
    t += (i == 0 ? "" : ", ") + pretty(e);
    ++i;
  }
  t += "}";
  return t;
}

template <typename Key, typename Hash, typename Pred, typename Allocator>
std::string pretty(const std::unordered_set<Key, Hash, Pred, Allocator>& s) {
  std::string t = "{";
  for (int i = 0; const auto& e: s) {
    t += (i == 0 ? "" : ", ") + pretty(e);
    ++i;
  }
  t += "}";
  return t;
}

template <typename Key, typename Hash, typename Pred, typename Allocator>
std::string pretty(
    const std::unordered_multiset<Key, Hash, Pred, Allocator>& s) {
  std::string t = "{";
  for (int i = 0; const auto& e: s) {
    t += (i == 0 ? "" : ", ") + pretty(e);
    ++i;
  }
  t += "}";
  return t;
}

template <typename Key, typename T, typename Compare, typename Allocator>
std::string pretty(const std::map<Key, T, Compare, Allocator>& m) {
  std::string s = "{";
  for (int i = 0; const auto& [k, v]: m) {
    s += (i == 0 ? "" : ", ") + pretty(k) + ": " + pretty(v);
    ++i;
  }
  s += "}";
  return s;
}

template <typename Key, typename T, typename Compare, typename Allocator>
std::string pretty(const std::multimap<Key, T, Compare, Allocator>& m) {
  std::string s = "{";
  for (int i = 0; const auto& [k, v]: m) {
    s += (i == 0 ? "" : ", ") + pretty(k) + ": " + pretty(v);
    ++i;
  }
  s += "}";
  return s;
}

template <typename Key,
          typename T,
          typename Hash,
          typename Pred,
          typename Allocator>
std::string pretty(const std::unordered_map<Key, T, Hash, Pred, Allocator>& m) {
  std::string s = "{";
  for (int i = 0; const auto& [k, v]: m) {
    s += (i == 0 ? "" : ", ") + pretty(k) + ": " + pretty(v);
    ++i;
  }
  s += "}";
  return s;
}

template <typename Key,
          typename T,
          typename Hash,
          typename Pred,
          typename Allocator>
std::string pretty(
    const std::unordered_multimap<Key, T, Hash, Pred, Allocator>& m) {
  std::string s = "{";
  for (int i = 0; const auto& [k, v]: m) {
    s += (i == 0 ? "" : ", ") + pretty(k) + ": " + pretty(v);
    ++i;
  }
  s += "}";
  return s;
}
