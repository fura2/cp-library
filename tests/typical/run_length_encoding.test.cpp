#include "typical/run_length_encoding.hpp"

#include <cassert>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

int main() {
  assert(run_length_encoding(std::string{}).empty());
  assert((run_length_encoding(std::string{"aaabbcaa"}) ==
          std::vector<std::pair<char, int>>{
              {'a', 3}, {'b', 2}, {'c', 1}, {'a', 2}}));
  const std::vector<int> values = {4, 4, -1, -1, -1, 4};
  assert((run_length_encoding(values) ==
          std::vector<std::pair<int, int>>{{4, 2}, {-1, 3}, {4, 1}}));
  assert((run_length_encoding(std::vector<int>{7}) ==
          std::vector<std::pair<int, int>>{{7, 1}}));
  std::istringstream input{"1 1 2 3 3"};
  assert((run_length_encoding(std::ranges::istream_view<int>(input)) ==
          std::vector<std::pair<int, int>>{{1, 2}, {2, 1}, {3, 2}}));
}
