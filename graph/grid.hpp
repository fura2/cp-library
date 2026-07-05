#pragma once

#include <array>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "template/io.hpp"

class Grid {
 public:
  static constexpr std::array<int, 4> dx4 = {0, -1, 0, 1};
  static constexpr std::array<int, 4> dy4 = {1, 0, -1, 0};
  static constexpr std::array<int, 8> dx8 = {0, -1, -1, -1, 0, 1, 1, 1};
  static constexpr std::array<int, 8> dy8 = {1, 1, 0, -1, -1, -1, 0, 1};

  template <typename std::size_t N>
  struct Neighbors {
    const std::pair<int, int>* begin() const { return data.data(); }
    const std::pair<int, int>* end() const { return data.data() + size; }

   private:
    friend class Grid;

    std::size_t size = 0;
    std::array<std::pair<int, int>, N> data{};
  };

  Grid() = default;
  Grid(int h, int w): h{h}, w{w}, data(h, std::string(w, '.')) {}

  std::string& operator[](int i) { return data[i]; }
  const std::string& operator[](int i) const { return data[i]; }

  int height() const { return h; }
  int width() const { return w; }

  bool contains(int x, int y) const {
    return 0 <= x && x < h && 0 <= y && y < w;
  }

  Neighbors<4> neighbors4(int x, int y) const {
    Neighbors<4> nbd;
    for (int i = 0; i < 4; ++i) {
      int x2 = x + dx4[i], y2 = y + dy4[i];
      if (contains(x2, y2)) {
        nbd.data[nbd.size++] = {x2, y2};
      }
    }
    return nbd;
  };
  Neighbors<8> neighbors8(int x, int y) const {
    Neighbors<8> nbd;
    for (int i = 0; i < 8; ++i) {
      int x2 = x + dx8[i], y2 = y + dy8[i];
      if (contains(x2, y2)) {
        nbd.data[nbd.size++] = {x2, y2};
      }
    }
    return nbd;
  };

  int count(char c) const {
    int res = 0;
    for (int i = 0; i < h; ++i)
      for (int j = 0; j < w; ++j)
        if (data[i][j] == c) ++res;
    return res;
  }

  friend void output(const Grid& G) {
    for (int i = 0; i < G.h; ++i) output(G.data[i]);
  }

 private:
  friend struct Input<Grid>;

  int h, w;
  std::vector<std::string> data;
};

template <>
struct Input<Grid> {
  static Grid read(int h, int w) {
    Grid G(h, w);
    for (int i = 0; i < h; ++i) {
      G.data[i] = input<std::string>();
    }
    return G;
  }
};
