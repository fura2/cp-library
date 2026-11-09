#include "set_operation/zeta_transform.hpp"

#include <cassert>
#include <concepts>
#include <utility>
#include <vector>

#include "algebra/collection/min_monoid.hpp"
#include "mint/mod_int.hpp"

struct MoveOnlyNumber {
  int x;
  explicit MoveOnlyNumber(int x): x{x} {}
  MoveOnlyNumber(const MoveOnlyNumber&) = delete;
  MoveOnlyNumber& operator=(const MoveOnlyNumber&) = delete;
  MoveOnlyNumber(MoveOnlyNumber&&) = default;
  MoveOnlyNumber& operator=(MoveOnlyNumber&&) = default;
  MoveOnlyNumber& operator+=(const MoveOnlyNumber& other) {
    x += other.x;
    return *this;
  }
  int unwrap() const { return x; }
};

static_assert(std::movable<MoveOnlyNumber> &&
              !std::copy_constructible<MoveOnlyNumber>);

template <typename T>
std::vector<int> values(const std::vector<T>& f) {
  std::vector<int> result;
  for (const auto& x: f) result.push_back(x.unwrap());
  return result;
}

void check_numeric() {
  // The value-returning overloads must preserve non-const lvalue inputs.
  std::vector<int> f{3, -2, 0, 5, -1, 4, -2, 1};
  const std::vector<int> expected_sub{3, 1, 3, 6, 2, 4, 0, 8};
  const std::vector<int> expected_super{8, 8, 4, 6, 2, 5, -1, 1};
  assert(subset_zeta_transform(f) == expected_sub);
  assert(superset_zeta_transform(f) == expected_super);
  assert((f == std::vector<int>{3, -2, 0, 5, -1, 4, -2, 1}));

  auto sub = f, super = f;
  subset_zeta_transform_inplace(sub);
  superset_zeta_transform_inplace(super);
  assert(sub == expected_sub && super == expected_super);

  const std::vector<int> single{-7};
  assert(subset_zeta_transform(single) == single);
  assert(superset_zeta_transform(single) == single);

  // ModInt has inverse(), but this overload must still use addition.
  const std::vector<ModInt<7>> modular{6, 2, 0, 3};
  assert(
      (values(subset_zeta_transform(modular)) == std::vector<int>{6, 1, 6, 4}));
  assert((values(superset_zeta_transform(modular)) ==
          std::vector<int>{4, 5, 3, 3}));
}

void check_abstract() {
  // MinMonoid has operator* but no operator+=; aggregation is not addition.
  const std::vector<MinMonoid<int>> minima{8, 3, 5, 7};
  assert(
      (values(subset_zeta_transform(minima)) == std::vector<int>{8, 3, 5, 3}));
  assert((values(superset_zeta_transform(minima)) ==
          std::vector<int>{3, 3, 5, 7}));
  auto sub = minima, super = minima;
  subset_zeta_transform_inplace(sub);
  superset_zeta_transform_inplace(super);
  assert((values(sub) == std::vector<int>{8, 3, 5, 3}));
  assert((values(super) == std::vector<int>{3, 3, 5, 7}));

  const std::vector<MinMonoid<int>> single{-7};
  assert((values(subset_zeta_transform(single)) == std::vector<int>{-7}));
  assert((values(superset_zeta_transform(single)) == std::vector<int>{-7}));
}

void check_move_only() {
  // Copying the vector inside either wrapper would fail to compile.
  std::vector<MoveOnlyNumber> sub, super;
  for (int x: {2, -3, 5, 7}) {
    sub.emplace_back(x);
    super.emplace_back(x);
  }
  const auto sub_result = subset_zeta_transform(std::move(sub));
  const auto super_result = superset_zeta_transform(std::move(super));
  assert((values(sub_result) == std::vector<int>{2, -1, 7, 11}));
  assert((values(super_result) == std::vector<int>{11, 4, 12, 7}));
}

int main() {
  check_numeric();
  check_abstract();
  check_move_only();
}
