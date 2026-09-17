#include "util/permutation.hpp"

#include <cassert>
#include <vector>

int main() {
  const Permutation empty{0}, identity{4};
  assert(empty.size() == 0 && empty.sign() == 1 && empty.cycles().empty());
  assert((identity.unwrap() == std::vector<int>{0, 1, 2, 3}));
  assert(identity.sign() == 1);

  Permutation p{std::vector<int>{2, 0, 1, 4, 3}};
  assert(p.size() == 5 && p[0] == 2 && p.preimage(0) == 1);
  assert((p.inverse_unwrap() == std::vector<int>{1, 2, 0, 4, 3}));
  assert((p.cycle(0) == std::vector<int>{0, 2, 1}));
  assert((p.cycles() == std::vector<std::vector<int>>{{0, 2, 1}, {3, 4}}));
  assert(p.sign() == -1);
  assert((p * p.inverse()).unwrap() == Permutation{5}.unwrap());
  const Permutation q{std::vector<int>{1, 0, 2, 3, 4}};
  assert((p * q).unwrap() == (std::vector<int>{0, 2, 1, 4, 3}));
  assert((q * p).unwrap() == (std::vector<int>{2, 1, 0, 4, 3}));
  assert((p * q).sign() == 1);

  p.swap(0, 1);
  assert(p[0] == 0 && p[1] == 2 && p.preimage(2) == 1 && p.sign() == 1);
  p.swap(0, 0);
  assert(p.sign() == 1);
  const auto inv = p.inverse();
  p.invert();
  assert(p.unwrap() == inv.unwrap() && p.sign() == inv.sign());
  // Also swap before sign() has populated the cached parity.
  Permutation uncached{std::vector<int>{1, 0, 2}};
  uncached.swap(0, 1);
  assert(uncached.sign() == 1);
}
