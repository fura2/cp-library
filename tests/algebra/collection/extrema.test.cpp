#include <cassert>
#include <type_traits>
#include <utility>

#include "algebra/collection/argmax_monoid.hpp"
#include "algebra/collection/argmin_monoid.hpp"
#include "algebra/collection/max_monoid.hpp"
#include "algebra/collection/min_monoid.hpp"
#include "algebra/monoid.hpp"

template <typename T>
concept ValidMin = requires { typename MinMonoid<T>; };
template <typename T>
concept ValidMax = requires { typename MaxMonoid<T>; };
template <typename T>
concept ValidArgmin = requires { typename ArgminMonoid<T>; };
template <typename T>
concept ValidArgmax = requires { typename ArgmaxMonoid<T>; };

struct Ordered {
  int value;
  friend bool operator<(const Ordered& a, const Ordered& b) {
    return a.value < b.value;
  }
};

template <>
inline constexpr auto id_inf<Ordered> = []() { return Ordered{INF}; };

static_assert(ValidMin<int> && ValidMax<int>);
static_assert(ValidArgmin<int> && ValidArgmax<int>);
static_assert(!ValidMin<float> && !ValidMax<float>);
static_assert(!ValidArgmin<float> && !ValidArgmax<float>);
// Having +infinity is insufficient when unary minus is unavailable.
static_assert(ValidMin<Ordered> && ValidArgmin<Ordered>);
static_assert(!ValidMax<Ordered> && !ValidArgmax<Ordered>);

template <typename Min, typename Max, typename Argmin, typename Argmax>
void check() {
  static_assert(Monoid<Min> && Monoid<Max> && Monoid<Argmin> && Monoid<Argmax>);
  using T = std::remove_cvref_t<decltype(Min{}.unwrap())>;
  const Min low{T{2}}, high{T{5}};
  const Max low_max{T{2}}, high_max{T{5}};
  assert((low * high).unwrap() == 2 && (high * low).unwrap() == 2);
  assert((low_max * high_max).unwrap() == 5);
  assert((high_max * low_max).unwrap() == 5);
  assert((Min::identity() * low).unwrap() == 2);
  assert((low * Min::identity()).unwrap() == 2);
  assert((Max::identity() * high_max).unwrap() == 5);
  assert((high_max * Max::identity()).unwrap() == 5);

  const Argmin a{std::pair{T{2}, 3}}, b{std::pair{T{5}, 7}};
  const Argmax x{std::pair{T{2}, 3}}, y{std::pair{T{5}, 7}};
  assert((a * b).unwrap() == a.unwrap() && (b * a).unwrap() == a.unwrap());
  assert((x * y).unwrap() == y.unwrap() && (y * x).unwrap() == y.unwrap());
  assert((Argmin::identity() * a).unwrap() == a.unwrap());
  assert((a * Argmin::identity()).unwrap() == a.unwrap());
  assert((Argmax::identity() * y).unwrap() == y.unwrap());
  assert((y * Argmax::identity()).unwrap() == y.unwrap());
  // Ties keep the left operand's index.
  const Argmin tied_min{std::pair{T{2}, 9}};
  const Argmax tied_max{std::pair{T{5}, 9}};
  assert((a * tied_min).unwrap().second == 3);
  assert((tied_min * a).unwrap().second == 9);
  assert((y * tied_max).unwrap().second == 7);
  assert((tied_max * y).unwrap().second == 9);
  assert(Argmin::identity().unwrap().second == -1);
  assert(Argmax::identity().unwrap().second == -1);
}

int main() {
  check<MinMonoid<int>, MaxMonoid<int>, ArgminMonoid<int>, ArgmaxMonoid<int>>();
  check<MinMonoid<long long>,
        MaxMonoid<long long>,
        ArgminMonoid<long long>,
        ArgmaxMonoid<long long>>();
  check<MinMonoid<double>,
        MaxMonoid<double>,
        ArgminMonoid<double>,
        ArgmaxMonoid<double>>();
  assert(MinMonoid<int>::identity().unwrap() == INF);
  assert(MaxMonoid<long long>::identity().unwrap() == -LINF);
  assert(MinMonoid<double>::identity().unwrap() == DINF);
}
