#pragma once

#include <concepts>
#include <string>
#include <utility>

#include "algebra/ring.hpp"

template <Ring R>
class AdditiveGroupOfRing {
 public:
  constexpr AdditiveGroupOfRing(): r{R::zero()} {}
  constexpr AdditiveGroupOfRing(const R& r): r{r} {}
  constexpr AdditiveGroupOfRing(R&& r): r{std::move(r)} {}

  constexpr AdditiveGroupOfRing operator*(const AdditiveGroupOfRing& g) const {
    return AdditiveGroupOfRing{r + g.r};
  }
  static constexpr AdditiveGroupOfRing identity() {
    return AdditiveGroupOfRing{R::zero()};
  }
  constexpr AdditiveGroupOfRing inverse() const {
    return AdditiveGroupOfRing{-r};
  }

  decltype(auto) unwrap() const
    requires requires(const R& r) { r.unwrap(); }
  {
    return r.unwrap();
  }
  decltype(auto) unwrap()
    requires requires(R& r) { r.unwrap(); }
  {
    return r.unwrap();
  }

  friend std::string pretty(const AdditiveGroupOfRing& g)
    requires requires(const AdditiveGroupOfRing& g) {
      { pretty(g.unwrap()) } -> std::same_as<std::string>;
    }
  {
    return pretty(g.unwrap());
  }

 private:
  R r;
};
