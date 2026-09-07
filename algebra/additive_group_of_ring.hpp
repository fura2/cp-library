#pragma once

#include <concepts>
#include <string>
#include <utility>

#include "algebra/ring.hpp"

template <Ring R>
class AdditiveGroupOfRing {
 public:
  AdditiveGroupOfRing(): r{R::zero()} {}
  AdditiveGroupOfRing(const R& r): r{r} {}
  AdditiveGroupOfRing(R&& r): r{std::move(r)} {}

  friend AdditiveGroupOfRing operator*(const AdditiveGroupOfRing& g,
                                       const AdditiveGroupOfRing& h) {
    return AdditiveGroupOfRing{g.r + h.r};
  }
  static AdditiveGroupOfRing identity() {
    return AdditiveGroupOfRing{R::zero()};
  }
  AdditiveGroupOfRing inverse() const { return AdditiveGroupOfRing{-r}; }

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
