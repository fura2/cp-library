#include "typical/inversion_number.hpp"

#include <cassert>
#include <cstdint>
#include <vector>

int main() {
  assert(inversion_number(std::vector<int>{}) == 0);
  assert(inversion_number(std::vector<int>{42}) == 0);
  assert(inversion_number(std::vector<int>{3, -1, 3, 2, -1}) == 6);
  assert(inversion_number(std::vector<unsigned long long>{1ULL << 40, 0, 1}) ==
         2);

  // The largest compressed rank is 128, which does not fit in int8_t.
  std::vector<std::int8_t> increasing;
  for (int x = -1; x <= 127; ++x) increasing.push_back(x);
  assert(inversion_number(increasing) == 0);

  // Every true precedes every false, giving more inversions than INT_MAX.
  std::vector<bool> bits(50'000, true);
  bits.resize(100'000, false);
  assert(inversion_number(bits) == 50'000LL * 50'000);
}
