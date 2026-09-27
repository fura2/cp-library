#include "algebra/collection/id.hpp"

#include <cassert>
#include <concepts>
#include <string>

#include "algebra/collection/add_group.hpp"
#include "algebra/group.hpp"

template <typename T>
concept HasZero = requires {
  { id_zero<T>() } -> std::same_as<T>;
};
template <typename T>
concept HasOne = requires {
  { id_one<T>() } -> std::same_as<T>;
};
template <typename T>
concept HasInf = requires {
  { id_inf<T>() } -> std::same_as<T>;
};
template <typename T>
concept HasNegInf = requires {
  { id_neg_inf<T>() } -> std::same_as<T>;
};

static_assert(!HasZero<float> && !HasOne<float>);
static_assert(!HasInf<float> && !HasNegInf<float>);
static_assert(!HasZero<std::string> && !HasOne<std::string>);
static_assert(!HasInf<std::string> && !HasNegInf<std::string>);
static_assert(!HasZero<void> && !HasOne<void>);
static_assert(!HasInf<void> && !HasNegInf<void>);

struct Counter {
  int value;
  friend Counter operator+(const Counter& a, const Counter& b) {
    return {a.value + b.value};
  }
  Counter operator-() const { return {-value}; }
};

template <>
inline constexpr auto id_zero<Counter> = []() { return Counter{0}; };
template <>
inline constexpr auto id_inf<Counter> = []() { return Counter{100}; };

static_assert(HasZero<Counter> && HasInf<Counter> && HasNegInf<Counter>);
static_assert(!HasOne<Counter>);
static_assert(Group<AddGroup<Counter>>);

template <typename T>
void check(T infinity) {
  static_assert(HasZero<T> && HasOne<T> && HasInf<T> && HasNegInf<T>);
  assert(id_zero<T>() == T{0} && id_one<T>() == T{1});
  assert(id_inf<T>() == infinity && id_neg_inf<T>() == -infinity);
}

int main() {
  check<int>(INF);
  check<long long>(LINF);
  check<double>(DINF);
  assert(id_neg_inf<Counter>().value == -100);
  using G = AddGroup<Counter>;
  const G a{Counter{3}}, b{Counter{5}};
  assert(G::identity().unwrap().value == 0);
  assert((a * b).unwrap().value == 8);
  assert((a * a.inverse()).unwrap().value == 0);
}
