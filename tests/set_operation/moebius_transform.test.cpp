#include "set_operation/moebius_transform.hpp"

#include <cassert>
#include <concepts>
#include <utility>
#include <vector>

#include "algebra/collection/id.hpp"
#include "algebra/collection/op.hpp"
#include "algebra/group_impl.hpp"
#include "mint/mod_int.hpp"
#include "set_operation/zeta_transform.hpp"

struct MoveOnlyGroup {
  int x;
  explicit MoveOnlyGroup(int x): x{x} {}
  MoveOnlyGroup(const MoveOnlyGroup&) = delete;
  MoveOnlyGroup& operator=(const MoveOnlyGroup&) = delete;
  MoveOnlyGroup(MoveOnlyGroup&&) = default;
  MoveOnlyGroup& operator=(MoveOnlyGroup&&) = default;
  static MoveOnlyGroup identity() { return MoveOnlyGroup{0}; }
  MoveOnlyGroup inverse() const { return MoveOnlyGroup{-x}; }
  friend MoveOnlyGroup operator*(const MoveOnlyGroup& a,
                                 const MoveOnlyGroup& b) {
    return MoveOnlyGroup{a.x + b.x};
  }
  int unwrap() const { return x; }
};

static_assert(CommutativeGroup<MoveOnlyGroup> && std::movable<MoveOnlyGroup> &&
              !std::copy_constructible<MoveOnlyGroup>);

template <typename T>
std::vector<int> values(const std::vector<T>& f) {
  std::vector<int> result;
  for (const auto& x: f) result.push_back(x.unwrap());
  return result;
}

void check_numeric() {
  // In particular, superset_moebius_transform(f) must not select a void
  // overload and modify this non-const input.
  std::vector<int> f{3, -2, 0, 5, -1, 4, -2, 1};
  const std::vector<int> expected_sub{3, -5, -3, 10, -4, 10, 2, -12};
  const std::vector<int> expected_super{12, -10, -2, 4, -2, 3, -3, 1};
  assert(subset_moebius_transform(f) == expected_sub);
  assert(superset_moebius_transform(f) == expected_super);
  assert(subset_moebius_transform(subset_zeta_transform(f)) == f);
  assert(subset_zeta_transform(subset_moebius_transform(f)) == f);
  assert(superset_moebius_transform(superset_zeta_transform(f)) == f);
  assert(superset_zeta_transform(superset_moebius_transform(f)) == f);
  assert((f == std::vector<int>{3, -2, 0, 5, -1, 4, -2, 1}));

  auto sub = f, super = f;
  subset_moebius_transform_inplace(sub);
  superset_moebius_transform_inplace(super);
  assert(sub == expected_sub && super == expected_super);

  const std::vector<int> single{-7};
  assert(subset_moebius_transform(single) == single);
  assert(superset_moebius_transform(single) == single);

  const std::vector<ModInt<7>> modular{6, 2, 0, 3};
  assert((values(subset_moebius_transform(modular)) ==
          std::vector<int>{6, 3, 1, 0}));
  assert((values(superset_moebius_transform(modular)) ==
          std::vector<int>{0, 6, 4, 3}));
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
  auto inplace_sub = f, inplace_super = f;
  subset_moebius_transform_inplace(inplace_sub);
  superset_moebius_transform_inplace(inplace_super);
  const std::vector<int> expected_sub{2, 5, 2, 1};
  const std::vector<int> expected_super{1, 4, 3, 6};
  for (int i = 0; i < 4; ++i) {
    assert(sub[i].unwrap().unwrap() == expected_sub[i]);
    assert(super[i].unwrap().unwrap() == expected_super[i]);
    assert(inplace_sub[i].unwrap().unwrap() == expected_sub[i]);
    assert(inplace_super[i].unwrap().unwrap() == expected_super[i]);
  }
  const std::vector<Product> single{Mint{2}};
  assert(subset_moebius_transform(single)[0].unwrap().unwrap() == 2);
  assert(superset_moebius_transform(single)[0].unwrap().unwrap() == 2);
}

void check_move_only() {
  // A const-reference parameter or a working copy would reject this group.
  std::vector<MoveOnlyGroup> sub, super;
  for (int x: {2, -3, 5, 7}) {
    sub.emplace_back(x);
    super.emplace_back(x);
  }
  const auto sub_result = subset_moebius_transform(std::move(sub));
  const auto super_result = superset_moebius_transform(std::move(super));
  assert((values(sub_result) == std::vector<int>{2, -5, 3, 7}));
  assert((values(super_result) == std::vector<int>{7, -10, -2, 7}));
}

int main() {
  check_numeric();
  check_multiplicative_group();
  check_move_only();
}
