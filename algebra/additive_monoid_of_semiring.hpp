#pragma once

#include <concepts>
#include <string>
#include <utility>

#include "algebra/semiring.hpp"

template <Semiring S>
class AdditiveMonoidOfSemiring {
 public:
  constexpr AdditiveMonoidOfSemiring(): s{S::zero()} {}
  constexpr AdditiveMonoidOfSemiring(const S& s): s{s} {}
  constexpr AdditiveMonoidOfSemiring(S&& s): s{std::move(s)} {}

  constexpr AdditiveMonoidOfSemiring operator*(
      const AdditiveMonoidOfSemiring& m) const {
    return AdditiveMonoidOfSemiring{s + m.s};
  }
  static constexpr AdditiveMonoidOfSemiring identity() {
    return AdditiveMonoidOfSemiring{S::zero()};
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

  friend std::string pretty(const AdditiveMonoidOfSemiring& m)
    requires requires(const AdditiveMonoidOfSemiring& m) {
      { pretty(m.unwrap()) } -> std::same_as<std::string>;
    }
  {
    return pretty(m.unwrap());
  }

 private:
  S s;
};
