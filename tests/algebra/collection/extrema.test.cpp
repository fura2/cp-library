#include <cassert>
#include <type_traits>
#include <utility>

#include "algebra/collection/argmax_monoid.hpp"
#include "algebra/collection/argmin_monoid.hpp"
#include "algebra/collection/max_monoid.hpp"
#include "algebra/collection/min_monoid.hpp"

template <typename Min, typename Max, typename Argmin, typename Argmax>
void check() {
  using T = std::remove_cvref_t<decltype(Min{}.unwrap())>;
  const Min low{T{2}}, high{T{5}};
  const Max low_max{T{2}}, high_max{T{5}};
  assert((low * high).unwrap() == 2 && (high * low).unwrap() == 2);
  assert((low_max * high_max).unwrap() == 5);
  assert((high_max * low_max).unwrap() == 5);
  assert((Min::identity() * low).unwrap() == 2);
  assert((low * Min::identity()).unwrap() == 2);
  assert((Max::identity() * high_max).unwrap() == 5);
  assert((high_max * Max::identity()).unwrap() == 5);

  const Argmin a{std::pair{T{2}, 3}}, b{std::pair{T{5}, 7}};
  const Argmax x{std::pair{T{2}, 3}}, y{std::pair{T{5}, 7}};
  assert((a * b).unwrap() == a.unwrap() && (b * a).unwrap() == a.unwrap());
  assert((x * y).unwrap() == y.unwrap() && (y * x).unwrap() == y.unwrap());
  assert((Argmin::identity() * a).unwrap() == a.unwrap());
  assert((a * Argmin::identity()).unwrap() == a.unwrap());
  assert((Argmax::identity() * y).unwrap() == y.unwrap());
  assert((y * Argmax::identity()).unwrap() == y.unwrap());
  // Ties keep the left operand's index.
  const Argmin tied_min{std::pair{T{2}, 9}};
  const Argmax tied_max{std::pair{T{5}, 9}};
  assert((a * tied_min).unwrap().second == 3);
  assert((tied_min * a).unwrap().second == 9);
  assert((y * tied_max).unwrap().second == 7);
  assert((tied_max * y).unwrap().second == 9);
  assert(Argmin::identity().unwrap().second == -1);
  assert(Argmax::identity().unwrap().second == -1);
}

int main() {
  check<IntMinMonoid, IntMaxMonoid, IntArgminMonoid, IntArgmaxMonoid>();
  check<LintMinMonoid, LintMaxMonoid, LintArgminMonoid, LintArgmaxMonoid>();
  check<DoubleMinMonoid,
        DoubleMaxMonoid,
        DoubleArgminMonoid,
        DoubleArgmaxMonoid>();
  assert(IntMinMonoid::identity().unwrap() == INF);
  assert(LintMaxMonoid::identity().unwrap() == -LINF);
  assert(DoubleMinMonoid::identity().unwrap() == DINF);
}
