#pragma once

#include <cassert>
#include <concepts>
#include <optional>
#include <string>
#include <utility>

#include "algebra/semigroup.hpp"

template <Semigroup S>
class UnitizationOfSemigroup {
 public:
  UnitizationOfSemigroup(): s{std::nullopt} {}
  UnitizationOfSemigroup(const S& s): s{s} {}
  UnitizationOfSemigroup(S&& s): s{std::move(s)} {}

  friend UnitizationOfSemigroup operator*(const UnitizationOfSemigroup& m,
                                          const UnitizationOfSemigroup& n) {
    if (!m.s.has_value()) return n;
    if (!n.s.has_value()) return m;
    return m.s.value() * n.s.value();
  }
  static UnitizationOfSemigroup identity() { return UnitizationOfSemigroup{}; }

  decltype(auto) unwrap() const
    requires requires(const S& s) { s.unwrap(); }
  {
    assert(s.has_value());
    return s.value().unwrap();
  }
  decltype(auto) unwrap()
    requires requires(S& s) { s.unwrap(); }
  {
    assert(s.has_value());
    return s.value().unwrap();
  }

  bool is_identity() const { return !s.has_value(); }

  friend std::string pretty(const UnitizationOfSemigroup& m)
    requires requires(const S& s) {
      { pretty(s) } -> std::same_as<std::string>;
    }
  {
    return pretty(m.s);
  }

 private:
  std::optional<S> s;
};
