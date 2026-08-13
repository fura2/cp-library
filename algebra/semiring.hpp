#pragma once

#include <concepts>

template <typename S>
concept Semiring = requires {
  { S::zero() } -> std::same_as<S>;
  { S::one() } -> std::same_as<S>;
} && requires(const S& a, const S& b) {
  { a + b } -> std::same_as<S>;
  { a * b } -> std::same_as<S>;
};
