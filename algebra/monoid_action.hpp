#pragma once

#include <concepts>

#include "algebra/monoid.hpp"

template <typename A>
concept MonoidAction =
    requires {
      typename A::value_monoid;
      typename A::action_monoid;
    } && Monoid<typename A::value_monoid> &&
    Monoid<typename A::action_monoid> &&
    requires(const typename A::value_monoid& m,
             const typename A::action_monoid& f) {
      { A::act(m, f) } -> std::same_as<typename A::value_monoid>;
    };
