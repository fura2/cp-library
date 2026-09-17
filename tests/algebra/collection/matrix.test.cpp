#include <cassert>

#include "algebra/collection/add_mul_ring.hpp"
#include "algebra/collection/matrix_ring.hpp"
#include "algebra/collection/matrix_semiring.hpp"
#include "algebra/collection/min_plus_semiring.hpp"

int main() {
  using R = MatrixRing<2, IntRing>;
  using S = MatrixSemiring<2, IntMinPlusSemiring>;
  static_assert(Ring<R> && Semiring<S> && !Ring<S>);
  const IntRing raw[2][2] = {{1, 2}, {3, 4}};
  const R a{FixedSquareMatrix<2, IntRing>{raw}};
  assert(R::zero().unwrap()[0][0].unwrap() == 0);
  assert(R::one().unwrap()[0][0].unwrap() == 1);
  assert((a + a).unwrap()[1][0].unwrap() == 6);
  assert((a - a).unwrap()[0][1].unwrap() == 0);
  assert((-a).unwrap()[1][1].unwrap() == -4);
  assert((a * a).unwrap()[0][1].unwrap() == 10);
  assert((R::one() * a).unwrap()[0][1].unwrap() == 2);
  const IntMinPlusSemiring costs[2][2] = {{0, 3}, {INF, 0}};
  const S s{FixedSquareMatrix<2, IntMinPlusSemiring>{costs}};
  assert(S::zero().unwrap()[0][0].unwrap() == INF);
  assert(S::one().unwrap()[0][0].unwrap() == 0);
  assert((s + S::zero()).unwrap()[0][1].unwrap() == 3);
  assert((s * s).unwrap()[0][1].unwrap() == 3);
  assert((S::one() * s).unwrap()[1][0].unwrap() == INF);
}
