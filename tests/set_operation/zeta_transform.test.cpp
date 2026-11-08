#include "set_operation/zeta_transform.hpp"

#include <cassert>
#include <vector>

#include "algebra/collection/add_group.hpp"
#include "algebra/collection/min_monoid.hpp"
#include "mint/mod_int.hpp"

template <typename T>
std::vector<int> values(const std::vector<T>& f) {
  std::vector<int> result;
  for (const auto& x: f) result.push_back(x.unwrap());
  return result;
}

void check_numeric() {
  const std::vector<int> f{3, -2, 0, 5, -1, 4, -2, 1};
  assert(
      (subset_zeta_transform(f) == std::vector<int>{3, 1, 3, 6, 2, 4, 0, 8}));
  assert((superset_zeta_transform(f) ==
          std::vector<int>{8, 8, 4, 6, 2, 5, -1, 1}));
  assert((f == std::vector<int>{3, -2, 0, 5, -1, 4, -2, 1}));

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
  // These wrappers have operator* but no operator+=.
  const std::vector<AddGroup<int>> sums{2, -3, 5, 7};
  assert(
      (values(subset_zeta_transform(sums)) == std::vector<int>{2, -1, 7, 11}));
  assert((values(superset_zeta_transform(sums)) ==
          std::vector<int>{11, 4, 12, 7}));
  assert((values(sums) == std::vector<int>{2, -3, 5, 7}));

  const std::vector<MinMonoid<int>> minima{8, 3, 5, 7};
  assert(
      (values(subset_zeta_transform(minima)) == std::vector<int>{8, 3, 5, 3}));
  assert((values(superset_zeta_transform(minima)) ==
          std::vector<int>{3, 3, 5, 7}));

  const std::vector<MinMonoid<int>> single{-7};
  assert((values(subset_zeta_transform(single)) == std::vector<int>{-7}));
  assert((values(superset_zeta_transform(single)) == std::vector<int>{-7}));
}

int main() {
  check_numeric();
  check_abstract();
}
