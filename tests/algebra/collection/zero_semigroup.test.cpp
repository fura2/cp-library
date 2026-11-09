// Declare pretty overloads before templates that use them.
// clang-format off
#include "template/debug.hpp"
// clang-format on

#include <cassert>
#include <concepts>
#include <string>
#include <utility>

#include "algebra/collection/left_zero_semigroup.hpp"
#include "algebra/collection/right_zero_semigroup.hpp"
#include "algebra/monoid.hpp"

template <typename T>
void check(const T& first, const T& last) {
  // Keep aliases dependent on T to catch the GCC 15.2 lambda NTTP issue.
  using L = LeftZeroSemigroup<T>;
  using R = RightZeroSemigroup<T>;
  static_assert(Semigroup<L> && Semigroup<R>);
  static_assert(!Monoid<L> && !Monoid<R>);
  static_assert(!std::default_initializable<L> &&
                !std::default_initializable<R>);
  static_assert(std::same_as<L, L> && std::same_as<R, R>);
  static_assert(std::same_as<decltype(std::declval<L&>().unwrap()), T&>);
  static_assert(
      std::same_as<decltype(std::declval<const L&>().unwrap()), const T&>);
  static_assert(std::same_as<decltype(std::declval<R&>().unwrap()), T&>);
  static_assert(
      std::same_as<decltype(std::declval<const R&>().unwrap()), const T&>);

  const L la{first}, lb{last};
  const R ra{first}, rb{last};
  assert((la * lb).unwrap() == first && (lb * la).unwrap() == last);
  assert((ra * rb).unwrap() == last && (rb * ra).unwrap() == first);
  assert((la * la).unwrap() == first && (ra * ra).unwrap() == first);

  L a{la};
  R b{ra};
  a.unwrap() = last;
  b.unwrap() = last;
  assert(std::as_const(a).unwrap() == last);
  assert(std::as_const(b).unwrap() == last);
  assert(la.unwrap() == first && ra.unwrap() == first);
}

int main() {
  check<int>(-4, 7);
  check<std::string>("", "a string long enough to require allocation");
  assert(pretty(LeftZeroSemigroup<int>{3}) == "3");
  assert(pretty(RightZeroSemigroup<std::string>{std::string{"ab"}}) ==
         "\"ab\"");
}
