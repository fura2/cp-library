#include "template/template.hpp"

#define MULTI_TESTCASE

void testcase() {
  ;
  ;
}

int main() {
#ifndef MULTI_TESTCASE
  testcase();
#else
  int t = input();
  rep (i, t) {
    std::clog << "\033[32mCase #" << i + 1 << "\033[0m" << std::endl;
    testcase();
  }
#endif
  return 0;
}
