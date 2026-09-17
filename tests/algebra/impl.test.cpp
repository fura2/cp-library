// Declare pretty overloads before templates that use them.
// clang-format off
#include "template/debug.hpp"
// clang-format on

#include <cassert>
#include <string>
#include <utility>

#include "algebra/group_impl.hpp"
#include "algebra/monoid_impl.hpp"
#include "algebra/ring_impl.hpp"
#include "algebra/semigroup_impl.hpp"
#include "algebra/semiring_impl.hpp"

inline constexpr auto add = [](int a, int b) { return a + b; };
inline constexpr auto mul = [](int a, int b) { return a * b; };
inline constexpr auto zero = []() { return 0; };
inline constexpr auto one = []() { return 1; };
inline constexpr auto neg = [](int a) { return -a; };
using S = SemigroupImpl<int, add>;
using M = MonoidImpl<int, add, zero>;
using G = GroupImpl<int, add, zero, neg>;
using SR = SemiringImpl<int, add, mul, zero, one>;
using R = RingImpl<int, add, mul, zero, one, neg>;

template <typename T>
void check_unwrap() {
  const int value = 4;
  T x{value};
  assert(std::as_const(x).unwrap() == 4);
  x.unwrap() = 7;
  assert(std::as_const(x).unwrap() == 7);
  assert(pretty(x) == "7");
}

int main() {
  check_unwrap<S>();
  check_unwrap<M>();
  check_unwrap<G>();
  check_unwrap<SR>();
  check_unwrap<R>();
  assert((S{2} * S{3}).unwrap() == 5);
  assert(M{}.unwrap() == 0 && M::identity().unwrap() == 0);
  assert((M{2} * M{3}).unwrap() == 5);
  assert((M::identity() * M{3}).unwrap() == 3);
  assert((M{3} * M::identity()).unwrap() == 3);
  assert(G{}.unwrap() == 0 && G::identity().unwrap() == 0);
  assert((G{2} * G{3}).unwrap() == 5);
  assert(G{3}.inverse().unwrap() == -3);
  assert((G{3} * G{3}.inverse()).unwrap() == 0);
  assert(SR{}.unwrap() == 0 && SR::zero().unwrap() == 0);
  assert(SR::one().unwrap() == 1);
  assert((SR{2} + SR{3}).unwrap() == 5 && (SR{2} * SR{3}).unwrap() == 6);
  assert(R{}.unwrap() == 0 && R::zero().unwrap() == 0);
  assert(R::one().unwrap() == 1);
  assert((R{2} + R{3}).unwrap() == 5 && (R{2} * R{3}).unwrap() == 6);
  assert((R{2} - R{3}).unwrap() == -1 && (-R{3}).unwrap() == -3);

  using Concat = MonoidImpl<std::string,
                            [](const std::string& a, const std::string& b) {
                              return a + b;
                            },
                            []() { return std::string{}; }>;
  std::string text = "ab";
  const Concat a{text}, b{std::string{"cd"}};
  assert((a * b).unwrap() == "abcd" && (b * a).unwrap() == "cdab");
  assert(Concat{}.unwrap().empty());
}
