#pragma once

#include <bit>
#include <cassert>
#include <vector>

#include "algebra/monoid.hpp"

template <typename T>
void subset_zeta_transform_inplace(std::vector<T>& f) {
  assert(std::has_single_bit(f.size()));
  int n = std::bit_width(f.size()) - 1;

  for (int i = 0; i < n; ++i) {
    for (int S = 0; S < (1 << n); ++S) {
      if ((S >> i) & 1) {
        f[S] += f[S ^ (1 << i)];
      }
    }
  }
}

template <CommutativeMonoid M>
void subset_zeta_transform_inplace(std::vector<M>& f) {
  assert(std::has_single_bit(f.size()));
  int n = std::bit_width(f.size()) - 1;

  for (int i = 0; i < n; ++i) {
    for (int S = 0; S < (1 << n); ++S) {
      if ((S >> i) & 1) {
        f[S] = f[S] * f[S ^ (1 << i)];
      }
    }
  }
}

template <typename T>
std::vector<T> subset_zeta_transform(std::vector<T> f) {
  subset_zeta_transform_inplace(f);
  return f;
}

template <typename T>
void superset_zeta_transform_inplace(std::vector<T>& f) {
  assert(std::has_single_bit(f.size()));
  int n = std::bit_width(f.size()) - 1;

  for (int i = 0; i < n; ++i) {
    for (int S = 0; S < (1 << n); ++S) {
      if (!((S >> i) & 1)) {
        f[S] += f[S ^ (1 << i)];
      }
    }
  }
}

template <CommutativeMonoid M>
void superset_zeta_transform_inplace(std::vector<M>& f) {
  assert(std::has_single_bit(f.size()));
  int n = std::bit_width(f.size()) - 1;

  for (int i = 0; i < n; ++i) {
    for (int S = 0; S < (1 << n); ++S) {
      if (!((S >> i) & 1)) {
        f[S] = f[S] * f[S ^ (1 << i)];
      }
    }
  }
}

template <typename T>
std::vector<T> superset_zeta_transform(std::vector<T> f) {
  superset_zeta_transform_inplace(f);
  return f;
}
