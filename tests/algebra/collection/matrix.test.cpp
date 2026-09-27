#include <cassert>
#include <concepts>

#include "algebra/collection/add_mul_ring.hpp"
#include "algebra/collection/matrix_ring.hpp"
#include "algebra/collection/matrix_semiring.hpp"
#include "algebra/collection/min_plus_semiring.hpp"

int main() {
  using R = MatrixRing<2, AddMulRing<int>>;
  using S = MatrixSemiring<2, MinPlusSemiring<int>>;
  using RM = FixedSquareMatrix<2, AddMulRing<int>>;
  using SM = FixedSquareMatrix<2, MinPlusSemiring<int>>;
  static_assert(std::same_as<R, AddMulRing<RM>>);
  static_assert(std::same_as<S, AddMulSemiring<SM>>);
  static_assert(Ring<R> && Semiring<S> && !Ring<S>);
  const AddMulRing<int> raw[2][2] = {{1, 2}, {3, 4}};
  const R a{FixedSquareMatrix<2, AddMulRing<int>>{raw}};
  assert(R::zero().unwrap()[0][0].unwrap() == 0);
  assert(R::one().unwrap()[0][0].unwrap() == 1);
  assert((a + a).unwrap()[1][0].unwrap() == 6);
  assert((a - a).unwrap()[0][1].unwrap() == 0);
  assert((-a).unwrap()[1][1].unwrap() == -4);
  assert((a * a).unwrap()[0][1].unwrap() == 10);
  assert((R::one() * a).unwrap()[0][1].unwrap() == 2);
  const AddMulRing<int> other[2][2] = {{0, 1}, {2, 0}};
  const R b{RM{other}};
  assert((a * b).unwrap()[0][0].unwrap() == 4);
  assert((b * a).unwrap()[0][0].unwrap() == 3);
  assert(id_zero<RM>()[0][0].unwrap() == 0);
  assert(id_one<RM>()[0][0].unwrap() == 1);
  const MinPlusSemiring<int> costs[2][2] = {{0, 3}, {INF, 0}};
  const S s{FixedSquareMatrix<2, MinPlusSemiring<int>>{costs}};
  assert(S::zero().unwrap()[0][0].unwrap() == INF);
  assert(S::one().unwrap()[0][0].unwrap() == 0);
  assert((s + S::zero()).unwrap()[0][1].unwrap() == 3);
  assert((s * s).unwrap()[0][1].unwrap() == 3);
  assert((S::one() * s).unwrap()[1][0].unwrap() == INF);
  assert(id_zero<SM>()[0][0].unwrap() == INF);
  assert(id_one<SM>()[0][0].unwrap() == 0);
  assert(id_one<SM>()[0][1].unwrap() == INF);

  using Nested = MatrixRing<2, R>;
  assert(Nested::zero().unwrap()[0][0].unwrap()[0][0].unwrap() == 0);
  assert(Nested::one().unwrap()[0][0].unwrap()[0][0].unwrap() == 1);
  assert(Nested::one().unwrap()[0][1].unwrap()[0][0].unwrap() == 0);
}
