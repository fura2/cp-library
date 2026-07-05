#pragma once

#include <format>
#include <iostream>

void testcase();

int main() {
#ifndef MULTI_TESTCASE
  testcase();
#else
  auto t = input<int>();
  rep (i, t) {
#ifdef LOCAL
    std::cerr << std::format("\033[36mCase #{}\033[0m\n", i + 1);
#endif
    testcase();
  }
#endif
  return 0;
}
