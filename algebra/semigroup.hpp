#pragma once

#include <concepts>

template <typename S>
concept Semigroup = requires(const S& a, const S& b) {
  { a * b } -> std::same_as<S>;
};

template <typename M>
concept IdempotentSemigroup = Semigroup<M>;
