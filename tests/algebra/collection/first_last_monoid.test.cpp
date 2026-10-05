// Declare pretty overloads before templates that use them.
// clang-format off
#include "template/debug.hpp"
// clang-format on

#include <cassert>
#include <concepts>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "algebra/collection/first_monoid.hpp"
#include "algebra/collection/last_monoid.hpp"
#include "algebra/monoid.hpp"

struct NonDefault {
  int value;
  NonDefault() = delete;
  explicit NonDefault(int x): value{x} {}
  bool operator==(const NonDefault&) const = default;
};

template <Monoid M>
void check_identity(const std::vector<M>& values) {
  const M e = M::identity();
  assert(!M{}.unwrap() && !e.unwrap());
  for (const M& x: values) {
    assert((e * x).unwrap() == x.unwrap());
    assert((x * e).unwrap() == x.unwrap());
    assert((x * x).unwrap() == x.unwrap());
  }
}

template <typename T>
void check_values(const T& a, const T& b) {
  // Keep the aliases dependent on T to catch the GCC 15.2 lambda NTTP issue.
  using F = FirstMonoid<T>;
  using L = LastMonoid<T>;
  static_assert(Monoid<F> && Monoid<L>);
  static_assert(std::same_as<F, F> && std::same_as<L, L>);
  static_assert(std::default_initializable<F> && std::default_initializable<L>);
  static_assert(
      std::same_as<decltype(std::declval<F&>().unwrap()), std::optional<T>&>);
  static_assert(std::same_as<decltype(std::declval<const F&>().unwrap()),
                             const std::optional<T>&>);
  static_assert(
      std::same_as<decltype(std::declval<L&>().unwrap()), std::optional<T>&>);
  static_assert(std::same_as<decltype(std::declval<const L&>().unwrap()),
                             const std::optional<T>&>);

  const F fa{a}, fb{b};
  const L la{a}, lb{b};
  assert(fa.unwrap().has_value() && fa.unwrap() == a);
  assert(la.unwrap().has_value() && la.unwrap() == a);
  assert((fa * fb).unwrap() == a && (fb * fa).unwrap() == b);
  assert((la * lb).unwrap() == b && (lb * la).unwrap() == a);
  assert(!F{std::nullopt}.unwrap() && !L{std::nullopt}.unwrap());
  check_identity(std::vector<F>{F::identity(), fa, fb});
  check_identity(std::vector<L>{L::identity(), la, lb});

  const std::optional<T> source{a};
  F first{source};
  L last{std::optional<T>{b}};
  first.unwrap() = b;
  last.unwrap() = a;
  assert(std::as_const(first).unwrap() == b);
  assert(std::as_const(last).unwrap() == a);
  assert(source == a && fa.unwrap() == a && lb.unwrap() == b);
  first.unwrap().reset();
  last.unwrap().reset();
  assert(!first.unwrap() && !last.unwrap());
}

int main() {
  check_values<int>(0, 7);
  check_values<std::string>("", "a string long enough to require allocation");
  check_values<NonDefault>(NonDefault{3}, NonDefault{7});

  // A stored empty optional is a value, distinct from the added identity.
  using M = LastMonoid<std::optional<int>>;
  const M clear{std::optional<std::optional<int>>{std::in_place, std::nullopt}};
  const M set{std::optional<std::optional<int>>{std::in_place, 7}};
  const M cleared = set * clear, assigned = clear * set;
  assert(cleared.unwrap().has_value() && !cleared.unwrap()->has_value());
  assert(assigned.unwrap().has_value() && assigned.unwrap()->value() == 7);
  assert((clear * M::identity()).unwrap() == clear.unwrap());

  assert(pretty(FirstMonoid<int>{}) == "*");
  assert(pretty(LastMonoid<int>{0}) == "0");
  assert(pretty(FirstMonoid<std::string>{std::string{}}) == "\"\"");
  assert(pretty(LastMonoid<std::string>{std::string{"ab"}}) == "\"ab\"");
}
