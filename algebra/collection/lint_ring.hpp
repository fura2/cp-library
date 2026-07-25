#pragma once

#include "algebra/ring.hpp"

using LintRing = RingImpl<long long,
                          [](long long a, long long b) { return a + b; },
                          [](long long a, long long b) { return a * b; },
                          []() { return 0LL; },
                          []() { return 1LL; },
                          [](long long a) { return -a; }>;
