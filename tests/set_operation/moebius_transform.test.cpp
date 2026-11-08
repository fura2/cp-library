#include "set_operation/moebius_transform.hpp"

#include <cassert>
#include <vector>

#include "algebra/collection/add_group.hpp"
#include "algebra/collection/id.hpp"
#include "algebra/collection/op.hpp"
#include "algebra/group_impl.hpp"
#include "mint/mod_int.hpp"
#include "set_operation/zeta_transform.hpp"

template <typename T>
std::vector<int> values(const std::vector<T>& f) {
  std::vector<int> result;
  for (const auto& x: f) result.push_back(x.unwrap());
  return result;
}

void check_numeric() {
  const std::vector<int> f{3, -2, 0, 5, -1, 4, -2, 1};
  assert((subset_moebius_transform(f) ==
          std::vector<int>{3, -5, -3, 10, -4, 10, 2, -12}));
  assert((superset_moebius_transform(f) ==
          std::vector<int>{12, -10, -2, 4, -2, 3, -3, 1}));
  assert(subset_moebius_transform(subset_zeta_transform(f)) == f);
  assert(subset_zeta_transform(subset_moebius_transform(f)) == f);
  assert(superset_moebius_transform(superset_zeta_transform(f)) == f);
  assert(superset_zeta_transform(superset_moebius_transform(f)) == f);
  assert((f == std::vector<int>{3, -2, 0, 5, -1, 4, -2, 1}));

  const std::vector<int> single{-7};
  assert(subset_moebius_transform(single) == single);
  assert(superset_moebius_transform(single) == single);

  const std::vector<ModInt<7>> modular{6, 2, 0, 3};
  assert((values(subset_moebius_transform(modular)) ==
          std::vector<int>{6, 3, 1, 0}));
  assert((values(superset_moebius_transform(modular)) ==
          std::vector<int>{0, 6, 4, 3}));
}

void check_additive_group() {
  // AddGroup has neither operator+= nor operator-=.
  const std::vector<AddGroup<int>> f{2, -3, 5, 7};
  assert(
      (values(subset_moebius_transform(f)) == std::vector<int>{2, -5, 3, 7}));
  assert((values(superset_moebius_transform(f)) ==
          std::vector<int>{7, -10, -2, 7}));
  assert(values(subset_moebius_transform(subset_zeta_transform(f))) ==
         values(f));
  assert(values(subset_zeta_transform(subset_moebius_transform(f))) ==
         values(f));
  assert(values(superset_moebius_transform(superset_zeta_transform(f))) ==
         values(f));
  assert(values(superset_zeta_transform(superset_moebius_transform(f))) ==
         values(f));
  assert((values(f) == std::vector<int>{2, -3, 5, 7}));

  const std::vector<AddGroup<int>> single{-7};
  assert((values(subset_moebius_transform(single)) == std::vector<int>{-7}));
  assert((values(superset_moebius_transform(single)) == std::vector<int>{-7}));
}

void check_multiplicative_group() {
  using Mint = ModInt<7>;
  using Product =
      GroupImpl<Mint, op_mul<Mint>, id_one<Mint>, [](const Mint& x) {
        return x.inverse();
      }>;
  // Nonzero residues form a group; inverse() here is not additive negation.
  const std::vector<Product> f{Mint{2}, Mint{3}, Mint{4}, Mint{6}};
  const auto sub = subset_moebius_transform(f);
  const auto super = superset_moebius_transform(f);
  const std::vector<int> expected_sub{2, 5, 2, 1};
  const std::vector<int> expected_super{1, 4, 3, 6};
  for (int i = 0; i < 4; ++i) {
    assert(sub[i].unwrap().unwrap() == expected_sub[i]);
    assert(super[i].unwrap().unwrap() == expected_super[i]);
  }
}

int main() {
  check_numeric();
  check_additive_group();
  check_multiplicative_group();
}
