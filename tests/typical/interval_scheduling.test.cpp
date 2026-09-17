#include "typical/interval_scheduling.hpp"

#include <cassert>
#include <utility>
#include <vector>

int main() {
  assert(interval_scheduling(std::vector<std::pair<int, int>>{}).empty());
  const std::vector<std::pair<int, int>> intervals = {
      {3, 5}, {0, 2}, {1, 4}, {2, 3}, {5, 7}};
  assert((interval_scheduling(intervals) == std::vector<int>{1, 3, 0, 4}));
  assert(interval_scheduling(std::vector<std::pair<int, int>>{{1, 2}}) ==
         std::vector<int>{0});
  assert((interval_scheduling(std::vector<std::pair<long long, long long>>{
              {-5, -3}, {-3, -1}, {-4, 0}}) == std::vector<int>{0, 1}));
  const auto duplicate = interval_scheduling(
      std::vector<std::pair<int, int>>{{0, 2}, {0, 2}, {2, 3}});
  assert(duplicate.size() == 2);
  assert((duplicate[0] == 0 || duplicate[0] == 1) && duplicate[1] == 2);
}
