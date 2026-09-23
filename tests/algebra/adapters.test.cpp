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

// No unwrap(): the adapters should delegate to the ring's own pretty().
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

static_assert(Ring<PrintableRing>);
static_assert(HasPretty<AdditiveGroupOfRing<PrintableRing>>);
static_assert(HasPretty<AdditiveMonoidOfSemiring<PrintableRing>>);
static_assert(HasPretty<MultiplicativeMonoidOfSemiring<PrintableRing>>);

int main() {
  using Add = AdditiveMonoidOfSemiring<IntMinPlusSemiring>;
  using Mul = MultiplicativeMonoidOfSemiring<IntMinPlusSemiring>;
  using Group = AdditiveGroupOfRing<IntRing>;
  const IntMinPlusSemiring value{3};
  Add a{value};
  Mul m{value};
  const IntRing ring_value{3};
  Group g{ring_value};
  assert(Add{}.unwrap() == INF && Add::identity().unwrap() == INF);
  assert(Mul{}.unwrap() == 0 && Mul::identity().unwrap() == 0);
  assert(Group{}.unwrap() == 0 && Group::identity().unwrap() == 0);
  assert((a * Add{IntMinPlusSemiring{5}}).unwrap() == 3);
  assert((m * Mul{IntMinPlusSemiring{5}}).unwrap() == 8);
  assert((g * Group{IntRing{5}}).unwrap() == 8);
  assert(g.inverse().unwrap() == -3 && (g * g.inverse()).unwrap() == 0);
  assert((a * Add::identity()).unwrap() == 3);
  assert((Mul::identity() * m).unwrap() == 3);
  a.unwrap() = 7;
  m.unwrap() = 8;
  g.unwrap() = 9;
  assert(std::as_const(a).unwrap() == 7 && pretty(a) == "7");
  assert(std::as_const(m).unwrap() == 8 && pretty(m) == "8");
  assert(std::as_const(g).unwrap() == 9 && pretty(g) == "9");

  using PrettyAdd = AdditiveMonoidOfSemiring<PrintableRing>;
  using PrettyMul = MultiplicativeMonoidOfSemiring<PrintableRing>;
  using PrettyGroup = AdditiveGroupOfRing<PrintableRing>;
  assert(pretty(PrettyAdd{PrintableRing{3}}) == "ring:3");
  assert(pretty(PrettyMul{PrintableRing{3}}) == "ring:3");
  assert(pretty(PrettyGroup{PrintableRing{3}}) == "ring:3");
  assert(pretty(PrettyAdd::identity()) == "ring:0");
  assert(pretty(PrettyMul::identity()) == "ring:1");
  assert(pretty(PrettyGroup::identity()) == "ring:0");
}
