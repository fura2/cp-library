#pragma once

#include "algebra/ring_impl.hpp"
#include "mint/mint.hpp"

using MintRing = RingImpl<mint,
                          [](mint a, mint b) { return a + b; },
                          [](mint a, mint b) { return a * b; },
                          []() { return mint(0); },
                          []() { return mint(1); },
                          [](mint a) { return -a; }>;
