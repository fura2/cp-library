// Declare pretty overloads before templates that use them.
// clang-format off
#include "template/debug.hpp"
// clang-format on

#include <cassert>
#include <concepts>
#include <string>
#include <utility>

#include "algebra/additive_group_of_ring.hpp"
#include "algebra/additive_monoid_of_semiring.hpp"
#include "algebra/collection/add_mul_ring.hpp"
#include "algebra/collection/min_plus_semiring.hpp"
#include "algebra/multiplicative_monoid_of_semiring.hpp"

// No unwrap(): adapters expose the ring itself and delegate pretty() to it.
struct PrintableRing {
  int x;
  static PrintableRing zero() { return {0}; }
  static PrintableRing one() { return {1}; }
  friend PrintableRing operator+(PrintableRing a, PrintableRing b) {
    return {a.x + b.x};
  }
  friend PrintableRing operator*(PrintableRing a, PrintableRing b) {
    return {a.x * b.x};
  }
  friend PrintableRing operator-(PrintableRing a, PrintableRing b) {
    return {a.x - b.x};
  }
  PrintableRing operator-() const { return {-x}; }
  friend std::string pretty(const PrintableRing& r) {
    return "ring:" + std::to_string(r.x);
  }
};

template <typename T>
concept HasPretty = requires(const T& x) {
  { pretty(x) } -> std::same_as<std::string>;
};

template <typename Adapter, typename Inner>
concept UnwrapsTo = requires(Adapter& a, const Adapter& ca) {
  { a.unwrap() } -> std::same_as<Inner&>;
  { ca.unwrap() } -> std::same_as<const Inner&>;
};

static_assert(Ring<PrintableRing>);
static_assert(HasPretty<AdditiveGroupOfRing<PrintableRing>>);
static_assert(HasPretty<AdditiveMonoidOfSemiring<PrintableRing>>);
static_assert(HasPretty<MultiplicativeMonoidOfSemiring<PrintableRing>>);

int main() {
  using Add = AdditiveMonoidOfSemiring<MinPlusSemiring<int>>;
  using Mul = MultiplicativeMonoidOfSemiring<MinPlusSemiring<int>>;
  using Group = AdditiveGroupOfRing<AddMulRing<int>>;
  static_assert(UnwrapsTo<Add, MinPlusSemiring<int>>);
  static_assert(UnwrapsTo<Mul, MinPlusSemiring<int>>);
  static_assert(UnwrapsTo<Group, AddMulRing<int>>);
  const MinPlusSemiring<int> value{3};
  Add a{value};
  Mul m{value};
  const AddMulRing<int> ring_value{3};
  Group g{ring_value};
  assert(Add{}.unwrap().unwrap() == INF &&
         Add::identity().unwrap().unwrap() == INF);
  assert(Mul{}.unwrap().unwrap() == 0 &&
         Mul::identity().unwrap().unwrap() == 0);
  assert(Group{}.unwrap().unwrap() == 0 &&
         Group::identity().unwrap().unwrap() == 0);
  assert((a * Add{MinPlusSemiring<int>{5}}).unwrap().unwrap() == 3);
  assert((m * Mul{MinPlusSemiring<int>{5}}).unwrap().unwrap() == 8);
  assert((g * Group{AddMulRing<int>{5}}).unwrap().unwrap() == 8);
  assert(g.inverse().unwrap().unwrap() == -3 &&
         (g * g.inverse()).unwrap().unwrap() == 0);
  assert((a * Add::identity()).unwrap().unwrap() == 3);
  assert((Mul::identity() * m).unwrap().unwrap() == 3);
  a.unwrap() = MinPlusSemiring<int>{7};
  m.unwrap() = MinPlusSemiring<int>{8};
  g.unwrap() = AddMulRing<int>{9};
  assert(std::as_const(a).unwrap().unwrap() == 7 && pretty(a) == "7");
  assert(std::as_const(m).unwrap().unwrap() == 8 && pretty(m) == "8");
  assert(std::as_const(g).unwrap().unwrap() == 9 && pretty(g) == "9");

  using PrettyAdd = AdditiveMonoidOfSemiring<PrintableRing>;
  using PrettyMul = MultiplicativeMonoidOfSemiring<PrintableRing>;
  using PrettyGroup = AdditiveGroupOfRing<PrintableRing>;
  static_assert(UnwrapsTo<PrettyAdd, PrintableRing>);
  static_assert(UnwrapsTo<PrettyMul, PrintableRing>);
  static_assert(UnwrapsTo<PrettyGroup, PrintableRing>);
  PrettyAdd pretty_add{PrintableRing{3}};
  PrettyMul pretty_mul{PrintableRing{3}};
  PrettyGroup pretty_group{PrintableRing{3}};
  assert(pretty(pretty_add) == "ring:3");
  assert(pretty(pretty_mul) == "ring:3");
  assert(pretty(pretty_group) == "ring:3");
  assert(pretty(PrettyAdd::identity()) == "ring:0");
  assert(pretty(PrettyMul::identity()) == "ring:1");
  assert(pretty(PrettyGroup::identity()) == "ring:0");
  pretty_add.unwrap().x = 4;
  pretty_mul.unwrap().x = 5;
  pretty_group.unwrap().x = 6;
  assert(std::as_const(pretty_add).unwrap().x == 4 &&
         pretty(pretty_add) == "ring:4");
  assert(std::as_const(pretty_mul).unwrap().x == 5 &&
         pretty(pretty_mul) == "ring:5");
  assert(std::as_const(pretty_group).unwrap().x == 6 &&
         pretty(pretty_group) == "ring:6");
}
