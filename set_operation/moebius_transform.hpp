#pragma once

#include <bit>
#include <cassert>
#include <vector>

#include "algebra/group.hpp"

template <typename T>
std::vector<T> subset_moebius_transform(std::vector<T> f) {
  assert(std::has_single_bit(f.size()));
  int n = std::bit_width(f.size()) - 1;

  for (int i = 0; i < n; ++i) {
    for (int S = 0; S < (1 << n); ++S) {
      if ((S >> i) & 1) {
        f[S] -= f[S ^ (1 << i)];
      }
    }
  }
  return f;
}

template <CommutativeGroup G>
std::vector<G> subset_moebius_transform(std::vector<G> f) {
  assert(std::has_single_bit(f.size()));
  int n = std::bit_width(f.size()) - 1;

  for (int i = 0; i < n; ++i) {
    for (int S = 0; S < (1 << n); ++S) {
      if ((S >> i) & 1) {
        f[S] = f[S] * f[S ^ (1 << i)].inverse();
      }
    }
  }
  return f;
}

template <typename T>
std::vector<T> superset_moebius_transform(std::vector<T> f) {
  assert(std::has_single_bit(f.size()));
  int n = std::bit_width(f.size()) - 1;

  for (int i = 0; i < n; ++i) {
    for (int S = 0; S < (1 << n); ++S) {
      if (!((S >> i) & 1)) {
        f[S] -= f[S ^ (1 << i)];
      }
    }
  }
  return f;
}

template <CommutativeGroup G>
std::vector<G> superset_moebius_transform(std::vector<G> f) {
  assert(std::has_single_bit(f.size()));
  int n = std::bit_width(f.size()) - 1;

  for (int i = 0; i < n; ++i) {
    for (int S = 0; S < (1 << n); ++S) {
      if (!((S >> i) & 1)) {
        f[S] = f[S] * f[S ^ (1 << i)].inverse();
      }
    }
  }
  return f;
}
