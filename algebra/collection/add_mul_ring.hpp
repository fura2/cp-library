#pragma once

#include "algebra/ring_impl.hpp"

template <typename T, auto Zero, auto One>
  requires requires(const T& a) {
    { -a } -> std::same_as<T>;
  } && requires(const T& a, const T& b) {
    { a + b } -> std::same_as<T>;
    { a * b } -> std::same_as<T>;
  }
using AddMulRing = RingImpl<T,
                            [](const T& a, const T& b) { return a + b; },
                            [](const T& a, const T& b) { return a * b; },
                            Zero,
                            One,
                            [](const T& a) { return -a; }>;

using IntAddMulRing = AddMulRing<int, []() { return 0; }, []() { return 1; }>;
using LintAddMulRing =
    AddMulRing<long long, []() { return 0LL; }, []() { return 1LL; }>;
using DoubleAddMulRing =
    AddMulRing<double, []() { return 0.0; }, []() { return 1.0; }>;

using IntRing = IntAddMulRing;
using LintRing = LintAddMulRing;
using DoubleRing = DoubleAddMulRing;
