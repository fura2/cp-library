#pragma once

#include <concepts>

#include "algebra/semigroup.hpp"

template <typename M>
concept Monoid = Semigroup<M> && requires {
  { M::identity() } -> std::same_as<M>;
};

template <typename M>
concept CommutativeMonoid = Monoid<M>;
