#pragma once

#include <array>
#include <concepts>
#include <cstddef>
#include <string>
#include <utility>

#include "algebra/ring.hpp"
#include "algebra/semiring.hpp"

template <std::size_t N, Semiring S>
struct FixedSquareMatrix {
  using row_type = std::array<S, N>;
  using matrix_type = std::array<row_type, N>;

  FixedSquareMatrix() {
    for (auto& row: mat) row.fill(S::zero());
  }
  FixedSquareMatrix(const matrix_type& mat): mat{mat} {}
  FixedSquareMatrix(matrix_type&& mat): mat{std::move(mat)} {}
  FixedSquareMatrix(const S (&a)[N][N]) {
    for (auto i = 0uz; i < N; ++i) {
      for (auto j = 0uz; j < N; ++j) {
        mat[i][j] = a[i][j];
      }
    }
  }

  row_type& operator[](std::size_t i) { return mat[i]; }
  const row_type& operator[](std::size_t i) const { return mat[i]; }

  FixedSquareMatrix operator-() const
    requires Ring<S>
  {
    FixedSquareMatrix a;
    for (auto i = 0uz; i < N; ++i) {
      for (auto j = 0uz; j < N; ++j) {
        a.mat[i][j] = -mat[i][j];
      }
    }
    return a;
  }
  FixedSquareMatrix& operator+=(const FixedSquareMatrix& a) {
    for (auto i = 0uz; i < N; ++i) {
      for (auto j = 0uz; j < N; ++j) {
        mat[i][j] = mat[i][j] + a.mat[i][j];
      }
    }
    return *this;
  }
  FixedSquareMatrix& operator-=(const FixedSquareMatrix& a)
    requires Ring<S>
  {
    for (auto i = 0uz; i < N; ++i) {
      for (auto j = 0uz; j < N; ++j) {
        mat[i][j] = mat[i][j] - a.mat[i][j];
      }
    }
    return *this;
  }
  FixedSquareMatrix& operator*=(const FixedSquareMatrix& a) {
    FixedSquareMatrix b{};
    for (auto i = 0uz; i < N; ++i) {
      for (auto k = 0uz; k < N; ++k) {
        for (auto j = 0uz; j < N; ++j) {
          b.mat[i][j] = b.mat[i][j] + mat[i][k] * a.mat[k][j];
        }
      }
    }
    return *this = std::move(b);
  }
  friend FixedSquareMatrix operator+(FixedSquareMatrix a,
                                     const FixedSquareMatrix& b) {
    a += b;
    return a;
  }
  friend FixedSquareMatrix operator-(FixedSquareMatrix a,
                                     const FixedSquareMatrix& b)
    requires Ring<S>
  {
    a -= b;
    return a;
  }
  friend FixedSquareMatrix operator*(FixedSquareMatrix a,
                                     const FixedSquareMatrix& b) {
    a *= b;
    return a;
  }
  friend FixedSquareMatrix operator*(const S& c, const FixedSquareMatrix& a) {
    FixedSquareMatrix b{};
    for (auto i = 0uz; i < N; ++i) {
      for (auto j = 0uz; j < N; ++j) {
        b.mat[i][j] = c * a.mat[i][j];
      }
    }
    return b;
  }
  friend FixedSquareMatrix operator*(const FixedSquareMatrix& a, const S& c) {
    FixedSquareMatrix b{};
    for (auto i = 0uz; i < N; ++i) {
      for (auto j = 0uz; j < N; ++j) {
        b.mat[i][j] = a.mat[i][j] * c;
      }
    }
    return b;
  }

  friend bool operator==(const FixedSquareMatrix& a,
                         const FixedSquareMatrix& b) = default;

  const matrix_type& unwrap() const { return mat; }
  matrix_type& unwrap() { return mat; }

  static FixedSquareMatrix zero() { return FixedSquareMatrix{}; }
  static FixedSquareMatrix identity() {
    FixedSquareMatrix id{};
    for (auto i = 0uz; i < N; ++i) id[i][i] = S::one();
    return id;
  }

  friend std::string pretty(const FixedSquareMatrix& a)
    requires requires(const S& s) {
      { pretty(s) } -> std::same_as<std::string>;
    }
  {
    std::string s = "[";
    for (auto i = 0uz; i < N; ++i) {
      s += (i == 0 ? "" : ", ") + std::string("[");
      for (auto j = 0uz; j < N; ++j) {
        s += (j == 0 ? "" : ", ") + pretty(a[i][j]);
      }
      s += "]";
    }
    s += "]";
    return s;
  }

 private:
  matrix_type mat;
};
