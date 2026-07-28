#pragma once

#include "algebra/ring.hpp"

using IntRing = RingImpl<int,
                         [](int a, int b) { return a + b; },
                         [](int a, int b) { return a * b; },
                         []() { return 0; },
                         []() { return 1; },
                         [](int a) { return -a; }>;
