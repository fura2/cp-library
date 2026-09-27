#include <cassert>
#include <type_traits>

#include "algebra/collection/max_plus_semiring.hpp"
#include "algebra/collection/min_plus_semiring.hpp"
#include "algebra/semiring.hpp"

template <typename T>
concept ValidMinPlus = requires { typename MinPlusSemiring<T>; };

template <typename T>
concept ValidMaxPlus = requires { typename MaxPlusSemiring<T>; };

struct NoEquality {
  int value;
  friend bool operator<(const NoEquality& a, const NoEquality& b) {
    return a.value < b.value;
  }
  friend NoEquality operator+(const NoEquality& a, const NoEquality& b) {
    return {a.value + b.value};
  }
  NoEquality operator-() const { return {-value}; }
};

struct PromotedSum {
  int value;
  // Permit conversion from the sum, so only the exact-type constraint rejects
  // it.
  PromotedSum(int value): value{value} {}
  friend bool operator<(const PromotedSum& a, const PromotedSum& b) {
    return a.value < b.value;
  }
  friend bool operator==(const PromotedSum&, const PromotedSum&) = default;
  friend int operator+(const PromotedSum& a, const PromotedSum& b) {
    return a.value + b.value;
  }
  PromotedSum operator-() const { return {-value}; }
};

template <>
inline constexpr auto id_inf<NoEquality> = []() { return NoEquality{INF}; };
template <>
inline constexpr auto id_zero<NoEquality> = []() { return NoEquality{0}; };
template <>
inline constexpr auto id_inf<PromotedSum> = []() { return PromotedSum{INF}; };
template <>
inline constexpr auto id_zero<PromotedSum> = []() { return PromotedSum{0}; };

static_assert(ValidMinPlus<int> && ValidMaxPlus<int>);
static_assert(!ValidMinPlus<float> && !ValidMaxPlus<float>);
static_assert(!ValidMinPlus<NoEquality> && !ValidMaxPlus<NoEquality>);
static_assert(!ValidMinPlus<PromotedSum> && !ValidMaxPlus<PromotedSum>);

template <typename S>
void check(int expected_add) {
  static_assert(Semiring<S>);
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
  check<MinPlusSemiring<int>>(2);
  check<MinPlusSemiring<long long>>(2);
  check<MinPlusSemiring<double>>(2);
  check<MaxPlusSemiring<int>>(5);
  check<MaxPlusSemiring<long long>>(5);
  check<MaxPlusSemiring<double>>(5);
  assert(MinPlusSemiring<int>::zero().unwrap() == INF);
  assert(MaxPlusSemiring<int>::zero().unwrap() == -INF);
}
