#pragma once

#include <iostream>
#include <vector>

template <typename T>
struct Input {
  static T read() {
    T x;
    std::cin >> x;
    return x;
  }
};

template <typename T>
struct Input<std::vector<T>> {
  static std::vector<T> read(int n) {
    std::vector<T> v(n);
    for (int i = 0; i < n; ++i) {
      std::cin >> v[i];
    }
    return v;
  }

  static std::vector<T> read(int n, int offset) {
    std::vector<T> v(n);
    for (int i = 0; i < n; ++i) {
      std::cin >> v[i];
      v[i] -= offset;
    }
    return v;
  }
};

template <typename T, typename... Args>
T input(Args&&... args) {
  return Input<T>::read(std::forward<Args>(args)...);
}

inline void newline() {
#ifdef LOCAL
  std::cout << std::endl;
#else
  std::cout << "\n";
#endif
}

template <typename T>
void output(const T& x) {
  std::cout << x;
  newline();
}

template <typename T>
void output(const std::vector<T>& v) {
  for (int i = 0; const auto& x: v) {
    std::cout << (i == 0 ? "" : " ") << x;
    ++i;
  }
  newline();
}
