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

  const S& unwrap() const { return s; }
  S& unwrap() { return s; }

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
