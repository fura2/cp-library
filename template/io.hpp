#pragma once

#include <iostream>
#include <utility>
#include <vector>

template <typename T>
struct Input {
  static T read() {
    T x;
    std::cin >> x;
    return x;
  }
};

template <typename T, typename U>
struct Input<std::pair<T, U>> {
  static std::pair<T, U> read() {
    return std::pair{Input<T>::read(), Input<U>::read()};
  }
};

template <typename T>
struct Input<std::vector<T>> {
  static std::vector<T> read(int n) {
    std::vector<T> v;
    v.reserve(n);
    for (int i = 0; i < n; ++i) {
      v.push_back(Input<T>::read());
    }
    return v;
  }

  static std::vector<T> read(int n, int offset) {
    std::vector<T> v;
    v.reserve(n);
    for (int i = 0; i < n; ++i) {
      v.push_back(Input<T>::read() - offset);
    }
    return v;
  }
};

template <typename T, typename... Args>
T input(Args&&... args) {
  return Input<T>::read(std::forward<Args>(args)...);
}

template <typename T>
struct Output {
  static void write(const T& x) { std::cout << x; }
};

template <typename T, typename U>
struct Output<std::pair<T, U>> {
  static void write(const std::pair<T, U>& p) {
    Output<T>::write(p.first);
    std::cout << " ";
    Output<U>::write(p.second);
  }
};

template <typename T>
struct Output<std::vector<T>> {
  static void write(const std::vector<T>& v) {
    for (int i = 0; const auto& x: v) {
      if (i > 0) std::cout << " ";
      Output<T>::write(x);
      ++i;
    }
  }
};

template <typename T>
void output(const T& x) {
  Output<T>::write(x);
#ifdef LOCAL
  std::cout << std::endl;
#else
  std::cout << "\n";
#endif
}
