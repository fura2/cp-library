#pragma once

#include "algebra/collection/id.hpp"
#include "mint/mint.hpp"

template <>
inline constexpr auto id_zero<mint> = []() { return mint{0}; };

template <>
inline constexpr auto id_one<mint> = []() { return mint{1}; };
