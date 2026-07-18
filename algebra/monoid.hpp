#pragma once

#include <concepts>

template <typename M>
concept Monoid = requires {
  { M::identity() } -> std::same_as<M>;
} && requires(const M& a, const M& b) {
  { a * b } -> std::same_as<M>;
};

template <typename M>
concept CommutativeMonoid = Monoid<M>;
