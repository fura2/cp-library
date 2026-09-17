#include "typical/knapsack_01.hpp"

#include <cassert>
#include <vector>

int main() {
  for (auto algo: {KnapsackAlgorithm::WeightDp,
                   KnapsackAlgorithm::ValueDp,
                   KnapsackAlgorithm::MeetInTheMiddle}) {
    auto solve = [algo](std::vector<int> w, std::vector<int> v, int capacity) {
      return knapsack_01(w, v, capacity, algo);
    };
    assert(solve({}, {}, 0) == 0);
    assert(solve({2, 3, 4}, {4, 5, 7}, 5) == 9);
    assert(solve({2}, {3}, 4) == 3);  // Each item can be used only once.
    assert(solve({1, 2}, {3, 4}, 0) == 0);
    assert(solve({8, 9}, {100, 200}, 7) == 0);
    assert(solve({0, 2, 3}, {5, 4, 7}, 3) == 12);
    assert(solve({0, 1, 2}, {0, -3, 4}, 2) == 4);
    assert(solve({-2, 4}, {3, 8}, 2) == 11);
    assert(solve({-2, 4}, {-3, 8}, 2) == 5);
    assert(solve({-2}, {-3}, 0) == 0);
    assert((knapsack_01(std::vector<long long>{2, 3},
                        std::vector<long long>{4, 5},
                        5LL,
                        algo) == 9));
  }
}
