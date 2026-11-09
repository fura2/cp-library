#pragma once

#include <iterator>
#include <utility>
#include <vector>

#include "algebra/additive_group_of_ring.hpp"
#include "algebra/ring.hpp"
#include "set_operation/moebius_transform.hpp"
#include "set_operation/zeta_transform.hpp"

template <typename T>
std::vector<T> or_convolution(std::vector<T> a, std::vector<T> b) {
  assert(a.size() == b.size());
  int n = a.size();

  subset_zeta_transform_inplace(a);
  subset_zeta_transform_inplace(b);
  for (int i = 0; i < n; ++i) {
    a[i] = a[i] * b[i];
  }
  subset_moebius_transform_inplace(a);
  return a;
}

template <Ring R>
std::vector<R> or_convolution(std::vector<R> a, std::vector<R> b) {
  using G = AdditiveGroupOfRing<R>;
  assert(a.size() == b.size());
  int n = a.size();

  auto za = subset_zeta_transform(std::vector<G>(
      std::make_move_iterator(a.begin()), std::make_move_iterator(a.end())));
  auto zb = subset_zeta_transform(std::vector<G>(
      std::make_move_iterator(b.begin()), std::make_move_iterator(b.end())));
  for (int i = 0; i < n; ++i) {
    za[i].unwrap() = za[i].unwrap() * zb[i].unwrap();
  }
  subset_moebius_transform_inplace(za);
  for (int i = 0; i < n; ++i) {
    a[i] = std::move(za[i].unwrap());
  }
  return a;
}

template <typename T>
std::vector<T> and_convolution(std::vector<T> a, std::vector<T> b) {
  assert(a.size() == b.size());
  int n = a.size();

  superset_zeta_transform_inplace(a);
  superset_zeta_transform_inplace(b);
  for (int i = 0; i < n; ++i) {
    a[i] = a[i] * b[i];
  }
  superset_moebius_transform_inplace(a);
  return a;
}

template <Ring R>
std::vector<R> and_convolution(std::vector<R> a, std::vector<R> b) {
  using G = AdditiveGroupOfRing<R>;
  assert(a.size() == b.size());
  int n = a.size();

  auto za = superset_zeta_transform(std::vector<G>(
      std::make_move_iterator(a.begin()), std::make_move_iterator(a.end())));
  auto zb = superset_zeta_transform(std::vector<G>(
      std::make_move_iterator(b.begin()), std::make_move_iterator(b.end())));
  for (int i = 0; i < n; ++i) {
    za[i].unwrap() = za[i].unwrap() * zb[i].unwrap();
  }
  superset_moebius_transform_inplace(za);
  for (int i = 0; i < n; ++i) {
    a[i] = std::move(za[i].unwrap());
  }
  return a;
}
