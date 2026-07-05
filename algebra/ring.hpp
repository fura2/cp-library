#pragma once

#include <concepts>

#include "algebra/semiring.hpp"

template <typename R>
concept Ring = Semiring<R> && requires(const R& a) {
  { -a } -> std::same_as<R>;
} && requires(const R& a, const R& b) {
  { a - b } -> std::same_as<R>;
} && requires(R a, const R& b) {
  { a -= b } -> std::same_as<R&>;
};
