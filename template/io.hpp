#pragma once

#include <iostream>
#include <vector>

struct InputValue {
  template <typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
};

struct InputVector {
  int n;

  template <typename T>
  operator std::vector<T>() const {
    std::vector<T> v(n);
    for (auto& x: v) std::cin >> x;
    return v;
  }
};

inline InputValue input() { return {}; }
inline InputVector input(int n) { return {n}; }

template <typename T>
void output(const T& x) {
  std::cout << x << std::endl;
}

template <typename T>
void output(const std::vector<T>& v) {
  for (int i = 0; const auto& x: v) {
    std::cout << (i == 0 ? "" : " ") << x;
    ++i;
  }
  std::cout << std::endl;
}
