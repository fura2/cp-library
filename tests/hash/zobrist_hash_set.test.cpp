#include "hash/zobrist_hash_set.hpp"

#include <cassert>
#include <cstddef>

struct FixedHash {
  std::size_t operator()(long long x) const { return Splitmix64::hash(x); }
};

template <typename Hash>
void check() {
  ZobristHashSet<long long, Hash> a, b;
  assert(a.size() == 0 && a.hash() == 0 && !a.contains(1));
  assert(!a.erase(1));
  assert(a.insert(1));
  const auto one = a.hash();
  assert(!a.insert(1) && a.hash() == one && a.size() == 1);
  assert(a.insert(2) && a.insert(-3));
  assert(b.insert(-3) && b.insert(1) && b.insert(2));
  assert(a.hash() == b.hash() && a.size() == 3);
  assert(a.contains(-3) && !a.contains(4));
  assert(!a.erase(4) && a.hash() == b.hash());
  assert(a.erase(2) && a.erase(-3));
  assert(a.hash() == one);
  assert(a.erase(1) && a.hash() == 0 && a.size() == 0);
  assert(a.insert(1) && a.hash() == one);
}

int main() {
  check<Splitmix64>();
  check<FixedHash>();
  ZobristHashSet<long long, FixedHash> s;
  s.insert(5);
  s.insert(9);
  assert(s.hash() == (Splitmix64::hash(5) ^ Splitmix64::hash(9)));
}
