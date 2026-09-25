#include "data_structure/addable_heap.hpp"

#include <cassert>
#include <sstream>
#include <vector>

int main() {
  AddableMinHeap<int> heap;
  assert(heap.empty() && heap.size() == 0);
  heap.add_all(10);  // Future insertions keep their supplied value.
  heap.push(3);
  heap.push(1);
  heap.push(1);
  assert(heap.top() == 1 && heap.size() == 3);
  heap.add_all(-4);
  assert(heap.top() == -3);
  heap.pop();
  assert(heap.top() == -3);
  heap.push_range(std::vector<int>{-10, 7});
  assert(heap.top() == -10);
  heap.pop();
  heap.pop();
  assert(heap.top() == -1);
  heap.pop();
  assert(heap.top() == 7);
  heap.pop();
  assert(heap.empty());

  heap.push_range({5, -2, 5});
  heap.push_range({});
  assert(heap.size() == 3 && heap.top() == -2);
  heap.add_all(2);
  assert(heap.top() == 0);
  heap.pop();
  assert(heap.top() == 7);
  heap.pop();
  assert(heap.top() == 7);
  heap.pop();
  assert(heap.empty());

  const std::vector<int> values = {3, 1, 4};
  AddableMinHeap<int> copied{values};
  assert(copied.top() == 1);
  AddableMaxHeap<long long> maximum{std::vector<long long>{3, 1, 4}};
  maximum.add_all(5);
  maximum.push(6);
  assert(maximum.top() == 9);
  maximum.pop();
  assert(maximum.top() == 8);
  std::istringstream in{"20 -1"};
  maximum.push_range(std::ranges::istream_view<long long>(in));
  assert(maximum.top() == 20);
}
