#include <cassert>
#include <type_traits>

#include "algebra/collection/max_plus_semiring.hpp"
#include "algebra/collection/min_plus_semiring.hpp"

template <typename T, auto Inf, auto Zero>
concept ValidMinPlus = requires { typename MinPlusSemiring<T, Inf, Zero>; };

template <typename T, auto NegInf, auto Zero>
concept ValidMaxPlus = requires { typename MaxPlusSemiring<T, NegInf, Zero>; };

inline constexpr auto inf = []() { return INF; };
inline constexpr auto neg_inf = []() { return -INF; };
inline constexpr auto zero = []() { return 0; };
inline constexpr auto lint_zero = []() { return 0LL; };

static_assert(ValidMinPlus<int, inf, zero>);
static_assert(ValidMaxPlus<int, neg_inf, zero>);
static_assert(!ValidMinPlus<int, inf, lint_zero>);
static_assert(!ValidMaxPlus<int, neg_inf, lint_zero>);
static_assert(!ValidMinPlus<int, lint_zero, zero>);
static_assert(!ValidMaxPlus<int, lint_zero, zero>);

template <typename S>
void check(int expected_add) {
  using T = std::remove_cvref_t<decltype(S{}.unwrap())>;
  const S a{T{2}}, b{T{5}};
  assert((a + b).unwrap() == expected_add);
  assert((a * b).unwrap() == 7);
  assert(S::one().unwrap() == 0);
  assert((S::zero() + a).unwrap() == 2 && (a + S::zero()).unwrap() == 2);
  assert((S::one() * a).unwrap() == 2 && (a * S::one()).unwrap() == 2);
  assert((S::zero() * a).unwrap() == S::zero().unwrap());
  assert((a * S::zero()).unwrap() == S::zero().unwrap());
  assert((S::zero() * S::zero()).unwrap() == S::zero().unwrap());
  assert(S{}.unwrap() == S::zero().unwrap());
}

int main() {
  check<IntMinPlusSemiring>(2);
  check<LintMinPlusSemiring>(2);
  check<DoubleMinPlusSemiring>(2);
  check<IntMaxPlusSemiring>(5);
  check<LintMaxPlusSemiring>(5);
  check<DoubleMaxPlusSemiring>(5);
  assert(IntMinPlusSemiring::zero().unwrap() == INF);
  assert(IntMaxPlusSemiring::zero().unwrap() == -INF);
}
