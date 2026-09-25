#pragma once

#include <concepts>

#include "algebra/monoid.hpp"

template <Monoid M, Monoid F, auto Act>
  requires requires(const M& m, const F& f) {
    { Act(m, f) } -> std::same_as<M>;
  }
class MonoidActionImpl {
 public:
  using value_monoid = M;
  using action_monoid = F;

  inline static const auto act = Act;
};
