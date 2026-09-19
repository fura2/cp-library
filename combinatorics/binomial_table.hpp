#pragma once

#include <algorithm>
#include <cassert>
#include <limits>
#include <vector>

class BinomialTable {
 public:
  explicit BinomialTable(int n): BinomialTable(n, n) {}

  explicit BinomialTable(int n, int r)
      : n{n}, r{r}, mod{-1}, table(n + 1, std::vector(r + 1, 0LL)) {
    assert(n >= 0);
    assert(r >= 0);
    for (int i = 0; i <= n; ++i) {
      table[i][0] = 1;
      for (int j = 1; j <= std::min(i, r); ++j) {
        table[i][j] = table[i - 1][j] + table[i - 1][j - 1];
      }
    }
  }

  explicit BinomialTable(int n, int r, long long mod)
      : n{n}, r{r}, mod{mod}, table(n + 1, std::vector(r + 1, 0LL)) {
    assert(mod > 0);
    for (int i = 0; i <= n; ++i) {
      table[i][0] = 1 % mod;
      for (int j = 1; j <= std::min(i, r); ++j) {
        table[i][j] = table[i - 1][j] + table[i - 1][j - 1];
        if (table[i][j] >= mod) table[i][j] -= mod;
      }
    }
  }

  long long choose(int n, int r) const {
    assert(0 <= n && n <= this->n);
    assert(0 <= r && r <= this->r);
    return table[n][r];
  }

  long long multinomial(std::vector<int>& a) {
    long long res = 1;
    if (mod != -1) {
      assert(mod <= std::numeric_limits<int>::max());
      res %= mod;
    }
    int k = a.size(), s = 0;
    for (int i = 0; i < k; ++i) {
      assert(0 <= a[i] && a[i] <= r);
      s += a[i];
      assert(s <= n);
      res *= table[s][a[i]];
      if (mod != -1) res %= mod;
    }
    return res;
  }

 private:
  int n, r;
  long long mod;
  std::vector<std::vector<long long>> table;
};
