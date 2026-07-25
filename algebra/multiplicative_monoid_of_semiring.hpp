#pragma once

#include <concepts>
#include <string>
#include <utility>

#include "algebra/semiring.hpp"

template <Semiring S>
class MultiplicativeMonoidOfSemiring {
 public:
  constexpr MultiplicativeMonoidOfSemiring(): s{S::one()} {}
  constexpr MultiplicativeMonoidOfSemiring(const S& s): s{s} {}
  constexpr MultiplicativeMonoidOfSemiring(S&& s): s{std::move(s)} {}

  constexpr MultiplicativeMonoidOfSemiring operator*(
      const MultiplicativeMonoidOfSemiring& m) const {
    return MultiplicativeMonoidOfSemiring{s * m.s};
  }
  static constexpr MultiplicativeMonoidOfSemiring identity() {
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
    requires requires(const MultiplicativeMonoidOfSemiring& m) {
      { pretty(m.unwrap()) } -> std::same_as<std::string>;
    }
  {
    return pretty(m.unwrap());
  }

 private:
  S s;
};
