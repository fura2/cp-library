#pragma once

#include <iostream>
#include <sstream>
#include <string>

// Redirect the standard streams for one test scope, then restore their state.
class TestIO {
 public:
  explicit TestIO(const std::string& input = "")
      : in{input},
        old_in{std::cin.rdbuf(in.rdbuf())},
        old_out{std::cout.rdbuf(out.rdbuf())},
        old_err{std::cerr.rdbuf(err.rdbuf())},
        out_flags{std::cout.flags()},
        out_precision{std::cout.precision()} {}

  TestIO(const TestIO&) = delete;
  TestIO& operator=(const TestIO&) = delete;

  ~TestIO() {
    std::cin.rdbuf(old_in);
    std::cout.rdbuf(old_out);
    std::cerr.rdbuf(old_err);
    std::cout.flags(out_flags);
    std::cout.precision(out_precision);
  }

  std::istringstream in;
  std::ostringstream out, err;

 private:
  std::streambuf *old_in, *old_out, *old_err;
  std::ios::fmtflags out_flags;
  std::streamsize out_precision;
};
