#include "template/template.hpp"

#include <cassert>
#include <string>
#include <type_traits>
#include <vector>

#include "tests/support/io.hpp"

static_assert(std::is_same_v<lint, long long>);
static_assert(std::is_same_v<uint, unsigned int>);
static_assert(std::is_same_v<ulint, unsigned long long>);

int main() {
  int count = 0;
  rep (3) ++count;
  rep (0) ++count;
  assert(count == 3);
  std::vector<int> values;
  rep (i, 3) values.push_back(i);
  assert((values == std::vector<int>{0, 1, 2}));
  values.clear();
  rep (i, 2, 5) values.push_back(i);
  assert((values == std::vector<int>{2, 3, 4}));
  values.clear();
  rep (i, 1, 7, 2) values.push_back(i);
  assert((values == std::vector<int>{1, 3, 5}));
  values.clear();
  rrep (i, 3) values.push_back(i);
  assert((values == std::vector<int>{2, 1, 0}));
  values.clear();
  rrep (i, 2, 5) values.push_back(i);
  assert((values == std::vector<int>{4, 3, 2}));
  values.clear();
  rrep (i, 1, 7, 2) values.push_back(i);
  assert((values == std::vector<int>{6, 4, 2}));
  rrep (i, 0) values.push_back(i);
  assert(values.size() == 3);
  assert(bit(10, 0) == 0 && bit(10, 1) == 1);
  assert(bit(1ULL << 63, 63) == 1);
  int x = 3;
  assert(chmax(x, 5) && x == 5);
  assert(!chmax(x, 5) && !chmax(x, 4));
  assert(chmin(x, 2) && x == 2);
  assert(!chmin(x, 2) && !chmin(x, 3));

  std::vector<int> unique_values;
  uniq(unique_values);
  assert(unique_values.empty());
  unique_values = {7};
  uniq(unique_values);
  assert((unique_values == std::vector<int>{7}));
  unique_values = {4, 4, 4};
  uniq(unique_values);
  assert((unique_values == std::vector<int>{4}));
  unique_values = {3, -1, 3, 0, -1, 2};
  uniq(unique_values);
  assert((unique_values == std::vector<int>{-1, 0, 2, 3}));
  uniq(unique_values);
  assert((unique_values == std::vector<int>{-1, 0, 2, 3}));

  std::vector<std::string> words{"b", "a", "b", ""};
  uniq(words);
  assert((words == std::vector<std::string>{"", "a", "b"}));
  std::vector<bool> flags{true, false, true};
  uniq(flags);
  assert((flags == std::vector<bool>{false, true}));

  TestIO io;
  assert(std::cin.tie() == nullptr);
  assert(std::cout.precision() == 12);
  yes();
  no();
  yesno(true);
  yesno(false);
  alice();
  bob();
  alicebob(true);
  alicebob(false);
  output(0.5);
  assert(io.out.str() ==
         "Yes\nNo\nYes\nNo\nAlice\nBob\nAlice\nBob\n0.500000000000\n");
  int calls = 0;
  show(++calls);
#ifdef LOCAL
  assert(calls == 1);
  assert(io.err.str() == "\033[33m++calls = 1\n\033[0m");
#else
  assert(calls == 0 && io.err.str().empty());
#endif
}
