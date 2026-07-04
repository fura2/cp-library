#pragma once

#include <array>
#include <cstddef>
#include <string>
#include <utility>

#include "algebra/semiring.hpp"

template <Semiring R, std::size_t N>
struct SquareMatrix {
  using scalar_type = R::value_type;
  using data_type = std::array<std::array<scalar_type, N>, N>;

  constexpr SquareMatrix() {
    for (auto& row: data) row.fill(R::zero());
  }
  constexpr explicit SquareMatrix(const data_type& data): data(data) {}
  constexpr explicit SquareMatrix(data_type&& data): data(std::move(data)) {}

  constexpr scalar_type& operator[](int i, int j) { return data[i][j]; }
  constexpr const scalar_type& operator[](int i, int j) const {
    return data[i][j];
  }

  constexpr SquareMatrix& operator+=(const SquareMatrix& a) {
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        (*this)[i, j] = R::add((*this)[i, j], a[i, j]);
      }
    }
    return *this;
  }
  constexpr SquareMatrix& operator*=(const SquareMatrix& a) {
    SquareMatrix res{};
    for (int i = 0; i < N; ++i) {
      for (int k = 0; k < N; ++k) {
        for (int j = 0; j < N; ++j) {
          res[i, j] = R::add(res[i, j], R::mul((*this)[i, k], a[k, j]));
        }
      }
    }
    return *this = std::move(res);
  }
  friend constexpr SquareMatrix operator+(const SquareMatrix& a,
                                          const SquareMatrix& b) {
    return SquareMatrix{a} += b;
  }
  friend constexpr SquareMatrix operator*(const SquareMatrix& a,
                                          const SquareMatrix& b) {
    return SquareMatrix{a} *= b;
  }
  friend constexpr SquareMatrix operator*(const SquareMatrix& a,
                                          const scalar_type& c) {
    SquareMatrix res;
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        res[i, j] = R::mul(a[i, j], c);
      }
    }
    return res;
  }
  friend constexpr SquareMatrix operator*(const scalar_type& c,
                                          const SquareMatrix& a) {
    SquareMatrix res;
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        res[i, j] = R::mul(c, a[i, j]);
      }
    }
    return res;
  }

  friend constexpr bool operator==(const SquareMatrix& a,
                                   const SquareMatrix& b) = default;

  static constexpr SquareMatrix zero() { return {}; }
  static constexpr SquareMatrix identity() {
    SquareMatrix res{};
    for (int i = 0; i < N; ++i) res[i, i] = R::one();
    return res;
  }

  friend std::string pretty(const SquareMatrix& a)
    requires requires(const data_type& x) {
      { pretty(x) } -> std::convertible_to<std::string>;
    }
  {
    return pretty(a.data);
  }

 private:
  data_type data;
};
