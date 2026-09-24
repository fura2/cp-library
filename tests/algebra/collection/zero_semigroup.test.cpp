// Declare pretty overloads before templates that use them.
// clang-format off
#include "template/debug.hpp"
// clang-format on

#include <cassert>
#include <concepts>
#include <string>
#include <utility>
#include <vector>

#include "algebra/collection/left_zero_semigroup.hpp"
#include "algebra/collection/right_zero_semigroup.hpp"
#include "algebra/monoid.hpp"

template <typename T>
void check(const std::vector<T>& values) {
  // Exercise dependent aliases as well as their use as vector element types.
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

  std::vector<L> left;
  std::vector<R> right;
  for (const T& x: values) {
    left.emplace_back(x);
    right.emplace_back(x);
  }
  for (int i = 0; i < int(values.size()); ++i) {
    for (int j = 0; j < int(values.size()); ++j) {
      assert((left[i] * left[j]).unwrap() == values[i]);
      assert((right[i] * right[j]).unwrap() == values[j]);
      for (int k = 0; k < int(values.size()); ++k) {
        assert(((left[i] * left[j]) * left[k]).unwrap() ==
               (left[i] * (left[j] * left[k])).unwrap());
        assert(((right[i] * right[j]) * right[k]).unwrap() ==
               (right[i] * (right[j] * right[k])).unwrap());
      }
    }
  }

  L a{left.front()};
  R b{right.front()};
  a.unwrap() = values.back();
  b.unwrap() = values.back();
  assert(std::as_const(a).unwrap() == values.back());
  assert(std::as_const(b).unwrap() == values.back());
  assert(left.front().unwrap() == values.front());
  assert(right.front().unwrap() == values.front());
}

int main() {
  check<int>({-4, 0, 7});
  check<std::string>({"", "a", "a string long enough to require allocation"});
  assert(pretty(LeftZeroSemigroup<int>{3}) == "3");
  assert(pretty(RightZeroSemigroup<std::string>{std::string{"ab"}}) ==
         "\"ab\"");
}
