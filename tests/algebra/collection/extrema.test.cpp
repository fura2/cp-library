#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <type_traits>
#include <utility>
#include <vector>

#include "algebra/collection/argmax_monoid.hpp"
#include "algebra/collection/argmin_monoid.hpp"
#include "algebra/collection/max_monoid.hpp"
#include "algebra/collection/min_monoid.hpp"
#include "algebra/collection/second_max_monoid.hpp"
#include "algebra/collection/second_min_monoid.hpp"
#include "algebra/monoid.hpp"

template <typename T>
concept ValidMin = requires { typename MinMonoid<T>; };
template <typename T>
concept ValidMax = requires { typename MaxMonoid<T>; };
template <typename T>
concept ValidArgmin = requires { typename ArgminMonoid<T>; };
template <typename T>
concept ValidArgmax = requires { typename ArgmaxMonoid<T>; };
template <typename T>
concept ValidSecondMin = requires { typename SecondMinMonoid<T>; };
template <typename T>
concept ValidSecondMax = requires { typename SecondMaxMonoid<T>; };

struct Ordered {
  int value;
  friend bool operator<(const Ordered& a, const Ordered& b) {
    return a.value < b.value;
  }
};

template <>
inline constexpr auto id_inf<Ordered> = []() { return Ordered{INF}; };

struct NoOrder {
  int value;
  friend NoOrder operator-(const NoOrder& x) { return NoOrder{-x.value}; }
};

template <>
inline constexpr auto id_inf<NoOrder> = []() { return NoOrder{INF}; };

static_assert(ValidMin<int> && ValidMax<int>);
static_assert(ValidArgmin<int> && ValidArgmax<int>);
static_assert(ValidSecondMin<int> && ValidSecondMax<int>);
static_assert(!ValidMin<float> && !ValidMax<float>);
static_assert(!ValidArgmin<float> && !ValidArgmax<float>);
// Unsupported types must make the concept false, not cause a hard error.
static_assert(!ValidSecondMin<float> && !ValidSecondMax<float>);
// Having +infinity is insufficient when unary minus is unavailable.
static_assert(ValidMin<Ordered> && ValidArgmin<Ordered>);
static_assert(!ValidMax<Ordered> && !ValidArgmax<Ordered>);
static_assert(ValidSecondMin<Ordered> && !ValidSecondMax<Ordered>);
// Both sentinels exist, so rejection must come from the missing comparison.
static_assert(std::is_same_v<decltype(id_inf<NoOrder>()), NoOrder>);
static_assert(std::is_same_v<decltype(id_neg_inf<NoOrder>()), NoOrder>);
static_assert(!ValidSecondMin<NoOrder> && !ValidSecondMax<NoOrder>);

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

template <typename M, typename Compare>
void check_second(int sentinel, Compare compare) {
  static_assert(Monoid<M>);
  const std::pair empty{sentinel, sentinel};
  assert(M{}.unwrap() == empty);
  assert(M::identity().unwrap() == empty);

  // Valid states: empty, singleton, and sorted pairs (including duplicates).
  std::vector<M> states{M::identity()};
  for (int x: {-3, 0, 2}) {
    states.emplace_back(std::pair{x, sentinel});
    for (int y: {-3, 0, 2}) {
      if (!compare(y, x)) states.emplace_back(std::pair{x, y});
    }
  }
  for (const M& a: states) {
    assert((a * M::identity()).unwrap() == a.unwrap());
    assert((M::identity() * a).unwrap() == a.unwrap());
    for (const M& b: states) {
      // Sorting all four entries gives an independent top-two oracle.
      std::array values{a.unwrap().first,
                        a.unwrap().second,
                        b.unwrap().first,
                        b.unwrap().second};
      std::sort(values.begin(), values.end(), compare);
      const std::pair expected{values[0], values[1]};
      assert((a * b).unwrap() == expected);
      for (const M& c: states) {
        assert(((a * b) * c).unwrap() == (a * (b * c)).unwrap());
      }
    }
  }
}

int main() {
  check_second<SecondMinMonoid<int>>(INF, std::less<int>{});
  check_second<SecondMaxMonoid<int>>(-INF, std::greater<int>{});
  static_assert(Monoid<SecondMinMonoid<long long>>);
  static_assert(Monoid<SecondMaxMonoid<long long>>);
  static_assert(Monoid<SecondMinMonoid<double>>);
  static_assert(Monoid<SecondMaxMonoid<double>>);
  assert((SecondMinMonoid<long long>::identity().unwrap() ==
          std::pair{LINF, LINF}));
  assert((SecondMaxMonoid<long long>::identity().unwrap() ==
          std::pair{-LINF, -LINF}));
  assert(
      (SecondMinMonoid<double>::identity().unwrap() == std::pair{DINF, DINF}));
  assert((SecondMaxMonoid<double>::identity().unwrap() ==
          std::pair{-DINF, -DINF}));
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
