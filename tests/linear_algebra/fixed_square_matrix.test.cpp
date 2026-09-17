// Declare pretty overloads before templates that use them.
// clang-format off
#include "template/debug.hpp"
// clang-format on

#include "linear_algebra/fixed_square_matrix.hpp"

#include <cassert>
#include <utility>

#include "algebra/collection/add_mul_ring.hpp"
#include "algebra/collection/min_plus_semiring.hpp"

using Matrix = FixedSquareMatrix<2, IntRing>;

void check(const Matrix& a, int a00, int a01, int a10, int a11) {
  assert(a[0][0].unwrap() == a00 && a[0][1].unwrap() == a01);
  assert(a[1][0].unwrap() == a10 && a[1][1].unwrap() == a11);
}

// Equality is available when the coefficient type supports equality.
struct BooleanSemiring {
  bool value = false;
  static BooleanSemiring zero() { return {false}; }
  static BooleanSemiring one() { return {true}; }
  friend BooleanSemiring operator+(BooleanSemiring a, BooleanSemiring b) {
    return {a.value || b.value};
  }
  friend BooleanSemiring operator*(BooleanSemiring a, BooleanSemiring b) {
    return {a.value && b.value};
  }
  friend bool operator==(BooleanSemiring, BooleanSemiring) = default;
};

int main() {
  const IntRing raw[2][2] = {{1, 2}, {3, 4}};
  const Matrix a{raw};
  const Matrix::matrix_type storage = {{{5, 6}, {7, 8}}};
  const Matrix b{storage};
  Matrix moved{Matrix::matrix_type{{{1, 2}, {3, 4}}}};
  check(moved, 1, 2, 3, 4);
  check(Matrix{}, 0, 0, 0, 0);
  check(Matrix::zero(), 0, 0, 0, 0);
  check(Matrix::identity(), 1, 0, 0, 1);
  check(a + b, 6, 8, 10, 12);
  check(a - b, -4, -4, -4, -4);
  check(-a, -1, -2, -3, -4);
  check(a * b, 19, 22, 43, 50);
  check(b * a, 23, 34, 31, 46);
  check(a * Matrix::identity(), 1, 2, 3, 4);
  check(Matrix::identity() * a, 1, 2, 3, 4);
  check(a * Matrix::zero(), 0, 0, 0, 0);
  check(IntRing{2} * a, 2, 4, 6, 8);
  check(a * IntRing{3}, 3, 6, 9, 12);
  moved += b;
  moved -= b;
  moved *= moved;
  check(moved, 7, 10, 15, 22);
  moved.unwrap()[0][0] = 9;
  assert(std::as_const(moved).unwrap()[0][0].unwrap() == 9);
  assert(pretty(a) == "[[1, 2], [3, 4]]");

  using Tropical = FixedSquareMatrix<2, IntMinPlusSemiring>;
  const IntMinPlusSemiring costs[2][2] = {{0, 3}, {INF, 0}};
  const Tropical t{costs};
  const auto product = t * t;
  assert(product[0][1].unwrap() == 3 && product[1][0].unwrap() == INF);
  assert(Tropical::zero()[0][0].unwrap() == INF);
  assert(Tropical::identity()[0][0].unwrap() == 0);
  using BoolMatrix = FixedSquareMatrix<2, BooleanSemiring>;
  assert(BoolMatrix::identity() * BoolMatrix::identity() ==
         BoolMatrix::identity());
  assert(BoolMatrix::zero() != BoolMatrix::identity());
  using Scalar = FixedSquareMatrix<1, IntRing>;
  assert((Scalar::identity() * Scalar::identity())[0][0].unwrap() == 1);
}
