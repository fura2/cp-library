#pragma once

#include <concepts>
#include <string>
#include <utility>

#include "algebra/semiring.hpp"

template <Semiring S>
class AdditiveMonoidOfSemiring {
 public:
  AdditiveMonoidOfSemiring(): s{S::zero()} {}
  AdditiveMonoidOfSemiring(const S& s): s{s} {}
  AdditiveMonoidOfSemiring(S&& s): s{std::move(s)} {}

  friend AdditiveMonoidOfSemiring operator*(const AdditiveMonoidOfSemiring& m,
                                            const AdditiveMonoidOfSemiring& n) {
    return AdditiveMonoidOfSemiring{m.s + n.s};
  }
  static AdditiveMonoidOfSemiring identity() {
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
    requires requires(const S& s) {
      { pretty(s) } -> std::same_as<std::string>;
    }
  {
    return pretty(m.s);
  }

 private:
  S s;
};
