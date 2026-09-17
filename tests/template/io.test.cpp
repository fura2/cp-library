#include "template/io.hpp"

#include <cassert>
#include <string>
#include <utility>
#include <vector>

#include "tests/support/io.hpp"

int main() {
  TestIO io{"42 hello 3 4 1 2 3 2 4 6 7 8 9 10"};
  assert(input<int>() == 42);
  assert(input<std::string>() == "hello");
  assert((input<std::pair<int, int>>() == std::pair{3, 4}));
  assert((input<std::vector<int>>(3) == std::vector<int>{1, 2, 3}));
  assert((input<std::vector<int>>(3, 1) == std::vector<int>{1, 3, 5}));
  assert(input<std::vector<int>>(0).empty());
  assert((input<std::vector<std::pair<int, int>>>(2) ==
          std::vector<std::pair<int, int>>{{7, 8}, {9, 10}}));

  output(42);
  output(std::pair{1, 2});
  output(std::vector<int>{3, 4});
  output(std::vector<int>{3, 4}, -1);
  output(std::vector<int>{});
  const int a[] = {5, 6};
  output(a);
  output(a, 1);
  output("hello");
  const char* text = "world";
  output(text);
  output(std::string{"string"});
  output(std::vector<std::pair<int, int>>{{7, 8}, {9, 10}});
  assert(io.out.str() ==
         "42\n1 2\n3 4\n2 3\n\n5 6\n6 7\nhello\nworld\n"
         "string\n7 8 9 10\n");
}
