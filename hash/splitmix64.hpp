#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>

struct Splitmix64 {
  static std::uint64_t hash(std::uint64_t x) {
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
  }

  std::size_t operator()(std::uint64_t x) const {
    static const std::uint64_t FIXED_RANDOM =
        std::chrono::steady_clock::now().time_since_epoch().count();
    return hash(x + FIXED_RANDOM);
  }
};
