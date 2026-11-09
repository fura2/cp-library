#pragma once

#include <bit>
#include <cassert>
#include <vector>

#include "algebra/group.hpp"

template <typename T>
void subset_moebius_transform_inplace(std::vector<T>& f) {
  assert(std::has_single_bit(f.size()));
  int n = std::bit_width(f.size()) - 1;

  for (int i = 0; i < n; ++i) {
    for (int S = 0; S < (1 << n); ++S) {
      if ((S >> i) & 1) {
        f[S] -= f[S ^ (1 << i)];
      }
    }
  }
}

template <CommutativeGroup G>
void subset_moebius_transform_inplace(std::vector<G>& f) {
  assert(std::has_single_bit(f.size()));
  int n = std::bit_width(f.size()) - 1;

  for (int i = 0; i < n; ++i) {
    for (int S = 0; S < (1 << n); ++S) {
      if ((S >> i) & 1) {
        f[S] = f[S] * f[S ^ (1 << i)].inverse();
      }
    }
  }
}

template <typename T>
std::vector<T> subset_moebius_transform(std::vector<T> f) {
  subset_moebius_transform_inplace(f);
  return f;
}

template <typename T>
void superset_moebius_transform_inplace(std::vector<T>& f) {
  assert(std::has_single_bit(f.size()));
  int n = std::bit_width(f.size()) - 1;

  for (int i = 0; i < n; ++i) {
    for (int S = 0; S < (1 << n); ++S) {
      if (!((S >> i) & 1)) {
        f[S] -= f[S ^ (1 << i)];
      }
    }
  }
}

template <CommutativeGroup G>
void superset_moebius_transform_inplace(std::vector<G>& f) {
  assert(std::has_single_bit(f.size()));
  int n = std::bit_width(f.size()) - 1;

  for (int i = 0; i < n; ++i) {
    for (int S = 0; S < (1 << n); ++S) {
      if (!((S >> i) & 1)) {
        f[S] = f[S] * f[S ^ (1 << i)].inverse();
      }
    }
  }
}

template <typename T>
std::vector<T> superset_moebius_transform(std::vector<T> f) {
  superset_moebius_transform_inplace(f);
  return f;
}
