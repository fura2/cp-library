#pragma once

#include "algebra/ring_impl.hpp"

using DoubleRing = RingImpl<double,
                            [](double a, double b) { return a + b; },
                            [](double a, double b) { return a * b; },
                            []() { return 0.0; },
                            []() { return 1.0; },
                            [](double a) { return -a; }>;
