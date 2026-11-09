#include "set_operation/bitwise_convolution.hpp"

#include <cassert>
#include <concepts>
#include <utility>
#include <vector>

#include "algebra/collection/add_mul_ring.hpp"
#include "algebra/collection/matrix_ring.hpp"
#include "algebra/monoid.hpp"
#include "algebra/ring.hpp"
#include "mint/mod_int.hpp"

template <typename T>
auto value(const T& x) {
  if constexpr (requires { x.unwrap(); }) {
    return value(x.unwrap());
  }
  else {
    return x;
  }
}

template <typename T>
void check(const std::vector<T>& a, const std::vector<T>& b) {
  const T zero = [] {
    if constexpr (Ring<T>)
      return T::zero();
    else
      return T{0};
  }();
  int n = a.size();
  std::vector<T> expected_or(n, zero), expected_and(n, zero);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      expected_or[i | j] = expected_or[i | j] + a[i] * b[j];
      expected_and[i & j] = expected_and[i & j] + a[i] * b[j];
    }
  }

  const auto original_a = a, original_b = b;
  const auto actual_or = or_convolution(a, b);
  const auto actual_and = and_convolution(a, b);
  assert(actual_or.size() == a.size() && actual_and.size() == a.size());
  for (int i = 0; i < n; ++i) {
    assert(value(actual_or[i]) == value(expected_or[i]));
    assert(value(actual_and[i]) == value(expected_and[i]));
    assert(value(a[i]) == value(original_a[i]));
    assert(value(b[i]) == value(original_b[i]));
  }
}

template <typename T>
void check_scalar() {
  for (int n: {1, 16}) {
    std::vector<T> a, b;
    for (int i = 0; i < n; ++i) {
      a.emplace_back((i * 5 + 3) % 11 - 5);
      b.emplace_back((i * 7 + 2) % 13 - 6);
    }
    check(a, b);
  }
}

void check_noncommutative() {
  using Scalar = AddMulRing<int>;
  using Matrix = FixedSquareMatrix<2, Scalar>;
  using R = MatrixRing<2, Scalar>;
  const Scalar raw_x[2][2] = {{0, 1}, {0, 0}};
  const Scalar raw_y[2][2] = {{0, 0}, {1, 0}};
  const std::vector<R> a{R::zero(), R{Matrix{raw_x}}};
  const std::vector<R> b{R{Matrix{raw_y}}, R::zero()};
  const auto result_or = or_convolution(a, b);
  const auto result_and = and_convolution(a, b);
  // E12 * E21 = E11; reversed multiplication would produce E22 instead.
  for (int i = 0; i < 2; ++i) {
    for (int r = 0; r < 2; ++r) {
      for (int c = 0; c < 2; ++c) {
        assert(result_or[i].unwrap()[r][c].unwrap() ==
               (i == 1 && r == 0 && c == 0));
        assert(result_and[i].unwrap()[r][c].unwrap() ==
               (i == 0 && r == 0 && c == 0));
      }
    }
  }
}

// No default constructor or copies: wrapping, transforms and extraction must
// consume values by move. identity() also checks Ring/Monoid overlap dispatch.
template <bool RingLike>
struct MoveOnlyScalar {
  int x;
  explicit MoveOnlyScalar(int x): x{x} {}
  MoveOnlyScalar(const MoveOnlyScalar&) = delete;
  MoveOnlyScalar& operator=(const MoveOnlyScalar&) = delete;
  MoveOnlyScalar(MoveOnlyScalar&&) = default;
  MoveOnlyScalar& operator=(MoveOnlyScalar&&) = default;
  static MoveOnlyScalar zero()
    requires RingLike
  {
    return MoveOnlyScalar{0};
  }
  static MoveOnlyScalar one()
    requires RingLike
  {
    return MoveOnlyScalar{1};
  }
  static MoveOnlyScalar identity()
    requires RingLike
  {
    return one();
  }
  MoveOnlyScalar operator-() const { return MoveOnlyScalar{-x}; }
  MoveOnlyScalar& operator+=(const MoveOnlyScalar& other)
    requires(!RingLike)
  {
    x += other.x;
    return *this;
  }
  MoveOnlyScalar& operator-=(const MoveOnlyScalar& other)
    requires(!RingLike)
  {
    x -= other.x;
    return *this;
  }
  friend MoveOnlyScalar operator+(const MoveOnlyScalar& a,
                                  const MoveOnlyScalar& b) {
    return MoveOnlyScalar{a.x + b.x};
  }
  friend MoveOnlyScalar operator-(const MoveOnlyScalar& a,
                                  const MoveOnlyScalar& b) {
    return MoveOnlyScalar{a.x - b.x};
  }
  friend MoveOnlyScalar operator*(const MoveOnlyScalar& a,
                                  const MoveOnlyScalar& b) {
    return MoveOnlyScalar{a.x * b.x};
  }
};

using MoveOnlyNumber = MoveOnlyScalar<false>;
using MoveOnlyRing = MoveOnlyScalar<true>;
static_assert(!Ring<MoveOnlyNumber> && !CommutativeMonoid<MoveOnlyNumber>);
static_assert(Ring<MoveOnlyRing> && CommutativeMonoid<MoveOnlyRing>);
static_assert(!std::default_initializable<MoveOnlyRing> &&
              !std::copy_constructible<MoveOnlyRing>);

template <typename T>
void check_move_only() {
  auto make_values = [](int x, int y) {
    std::vector<T> result;
    result.emplace_back(x);
    result.emplace_back(y);
    return result;
  };
  auto a = make_values(1, 2), b = make_values(3, 4);
  const auto result_or = or_convolution(std::move(a), std::move(b));
  assert(result_or.size() == 2);
  assert(result_or[0].x == 3 && result_or[1].x == 18);

  const auto result_and = and_convolution(make_values(1, 2), make_values(3, 4));
  assert(result_and.size() == 2);
  assert(result_and[0].x == 13 && result_and[1].x == 8);
}

int main() {
  check_scalar<long long>();
  check_scalar<ModInt<7>>();
  check_scalar<AddMulRing<long long>>();
  check<long long>({0, 0, 0, 0}, {3, -1, 5, 2});
  check<long long>({3, -1, 5, 2}, {0, 1, 0, 0});
  check_noncommutative();
  check_move_only<MoveOnlyNumber>();
  check_move_only<MoveOnlyRing>();
}
