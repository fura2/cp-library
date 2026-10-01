#pragma once

#include <concepts>

#include "algebra/monoid.hpp"

template <typename X, Monoid F, auto Act>
  requires requires(const X& x, const F& f) {
    { Act(x, f) } -> std::same_as<X>;
  }
class MonoidActionImpl {
 public:
  using value_type = X;
  using action_type = F;

  inline static const auto act = Act;
};
