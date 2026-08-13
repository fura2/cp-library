#pragma once

#include <concepts>

template <typename M>
concept Semigroup = requires(const M& a, const M& b) {
  { a * b } -> std::same_as<M>;
};

template <typename M>
concept IdempotentSemigroup = Semigroup<M>;
