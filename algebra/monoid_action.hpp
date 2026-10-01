#pragma once

#include <concepts>

#include "algebra/monoid.hpp"

template <typename A>
concept MonoidAction =
    requires {
      typename A::value_type;
      typename A::action_type;
    } && Monoid<typename A::action_type> &&
    requires(const typename A::value_type& x,
             const typename A::action_type& f) {
      { A::act(x, f) } -> std::same_as<typename A::value_type>;
    };

template <typename A>
concept MonoidActionOnMonoid =
    MonoidAction<A> && Monoid<typename A::value_type>;
