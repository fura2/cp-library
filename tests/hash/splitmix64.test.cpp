#include "hash/splitmix64.hpp"

#include <cassert>
#include <cstdint>
#include <limits>
#include <unordered_set>

int main() {
  assert(Splitmix64::hash(0) == 0xe220a8397b1dcdafULL);
  assert(Splitmix64::hash(1) == 0x910a2dec89025cc1ULL);
  const Splitmix64 a, b;
  for (std::uint64_t x: {0ULL, 1ULL, 42ULL}) {
    assert(a(x) == a(x));
    assert(a(x) == b(x));
  }
  const auto large = std::numeric_limits<std::uint64_t>::max();
  const std::unordered_set<std::uint64_t, Splitmix64> values = {0, 1, large};
  assert(values.size() == 3);
  assert(values.contains(large) && !values.contains(2));
}
