#pragma once

#include <concepts>
#include <string>
#include <utility>

#include "algebra/semiring.hpp"

template <Semiring S>
class MultiplicativeMonoidOfSemiring {
 public:
  MultiplicativeMonoidOfSemiring(): s{S::one()} {}
  MultiplicativeMonoidOfSemiring(const S& s): s{s} {}
  MultiplicativeMonoidOfSemiring(S&& s): s{std::move(s)} {}

  friend MultiplicativeMonoidOfSemiring operator*(
      const MultiplicativeMonoidOfSemiring& m,
      const MultiplicativeMonoidOfSemiring& n) {
    return MultiplicativeMonoidOfSemiring{m.s * n.s};
  }
  static MultiplicativeMonoidOfSemiring identity() {
    return MultiplicativeMonoidOfSemiring{S::one()};
  }

  decltype(auto) unwrap() const
    requires requires(const S& s) { s.unwrap(); }
  {
    return s.unwrap();
  }
  decltype(auto) unwrap()
    requires requires(S& s) { s.unwrap(); }
  {
    return s.unwrap();
  }

  friend std::string pretty(const MultiplicativeMonoidOfSemiring& m)
    requires requires(const S& s) {
      { pretty(s) } -> std::same_as<std::string>;
    }
  {
    return pretty(m.s);
  }

 private:
  S s;
};
