#pragma once

#include <concepts>

#include "algebra/monoid.hpp"

template <typename G>
concept Group = Monoid<G> && requires(const G& a) {
  { a.inverse() } -> std::same_as<G>;
};
