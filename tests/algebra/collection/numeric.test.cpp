#include <cassert>
#include <concepts>
#include <string>

#include "algebra/collection/add_group.hpp"
#include "algebra/collection/add_monoid.hpp"
#include "algebra/collection/add_mul_ring.hpp"
#include "algebra/collection/add_mul_semiring.hpp"
#include "algebra/collection/mul_monoid.hpp"
#include "algebra/group.hpp"
#include "algebra/monoid.hpp"
#include "algebra/ring.hpp"
#include "algebra/semiring.hpp"

template <typename T>
concept ValidAddMonoid = requires { typename AddMonoid<T>; };

template <typename T>
concept ValidAddGroup = requires { typename AddGroup<T>; };

template <typename T>
concept ValidMulMonoid = requires { typename MulMonoid<T>; };

template <typename T>
concept ValidAddMulRing = requires { typename AddMulRing<T>; };

template <typename T>
concept ValidAddMulSemiring = requires { typename AddMulSemiring<T>; };

template <>
inline constexpr auto id_zero<std::string> = []() { return std::string{}; };
using Concat = AddMonoid<std::string>;

// Deliberately mismatched identity types must not be implicitly converted.
template <>
inline constexpr auto id_zero<long double> = []() { return 0.0; };
template <>
inline constexpr auto id_one<long double> = []() { return 1.0L; };
template <>
inline constexpr auto id_zero<float> = []() { return 0.0F; };
template <>
inline constexpr auto id_one<float> = []() { return 1.0; };

static_assert(ValidAddMonoid<int> && ValidAddGroup<int>);
static_assert(ValidMulMonoid<int> && ValidAddMulRing<int>);
static_assert(ValidAddMulSemiring<int>);
static_assert(!ValidAddMonoid<long double> && !ValidAddGroup<long double>);
static_assert(ValidMulMonoid<long double> && ValidAddGroup<float>);
static_assert(!ValidMulMonoid<float>);
static_assert(!ValidAddMulRing<long double> && !ValidAddMulRing<float>);
static_assert(!ValidAddMulSemiring<long double> && !ValidAddMulSemiring<float>);
static_assert(!ValidAddMonoid<short> && !ValidAddGroup<short>);
static_assert(!ValidMulMonoid<short> && !ValidAddMulRing<short>);
static_assert(Monoid<Concat> && !Group<Concat>);
static_assert(!ValidAddGroup<std::string> && !ValidMulMonoid<std::string>);

template <typename T>
void check_numeric() {
  using R = AddMulRing<T>;
  using A = AddGroup<T>;
  using M = MulMonoid<T>;
  using Sum = AddMonoid<T>;
  using S = AddMulSemiring<T>;
  static_assert(Ring<R> && Group<A> && Monoid<M>);
  static_assert(Monoid<Sum> && !Group<Sum> && Semiring<S> && !Ring<S>);
  assert(R::zero().unwrap() == 0 && R::one().unwrap() == 1);
  assert((R{3} + R{4}).unwrap() == 7);
  assert((R{3} - R{4}).unwrap() == -1);
  assert((R{3} * R{4}).unwrap() == 12 && (-R{3}).unwrap() == -3);
  assert(A{}.unwrap() == 0 && M{}.unwrap() == 1);
  assert(A::identity().unwrap() == 0 && M::identity().unwrap() == 1);
  assert((A{3} * A{4}).unwrap() == 7);
  assert((M{3} * M{4}).unwrap() == 12);
  assert(A{3}.inverse().unwrap() == -3);
  assert((A{3} * A{3}.inverse()).unwrap() == 0);
  assert((Sum{3} * Sum{4}).unwrap() == 7);
  assert((Sum::identity() * Sum{3}).unwrap() == 3);
  assert(S{}.unwrap() == 0 && S::zero().unwrap() == 0);
  assert(S::one().unwrap() == 1);
  assert((S{3} + S{4}).unwrap() == 7 && (S{3} * S{4}).unwrap() == 12);
}

int main() {
  check_numeric<int>();
  check_numeric<long long>();
  check_numeric<double>();
  assert((AddMulRing<long long>{3'000'000'000LL} +
          AddMulRing<long long>{4'000'000'000LL})
             .unwrap() == 7'000'000'000LL);
  assert((AddMulRing<double>{0.5} * AddMulRing<double>{0.25}).unwrap() ==
         0.125);
  assert((AddGroup<long long>{3'000'000'000LL} *
          AddGroup<long long>{4'000'000'000LL})
             .unwrap() == 7'000'000'000LL);
  assert((MulMonoid<long long>{3'000'000'000LL} * MulMonoid<long long>{2})
             .unwrap() == 6'000'000'000LL);
  assert((AddGroup<double>{0.5} * AddGroup<double>{0.25}).unwrap() == 0.75);
  assert((MulMonoid<double>{0.5} * MulMonoid<double>{0.25}).unwrap() == 0.125);

  const Concat first{std::string{"ab"}}, last{std::string{"cd"}};
  assert(Concat{}.unwrap().empty());
  assert((first * last).unwrap() == "abcd");
  assert((last * first).unwrap() == "cdab");
  assert((first * Concat::identity()).unwrap() == "ab");
  assert((Concat::identity() * first).unwrap() == "ab");
}
