#pragma once

#include <cassert>
#include <numeric>
#include <vector>

#include "graph/graph.hpp"

inline bool erdos_gallai(std::vector<int> d) {
  int n = d.size();

  long long dsum = 0;
  std::vector<int> hist(n);
  for (int x: d) {
    if (x < 0 || n <= x) return false;
    dsum += x;
    ++hist[x];
  }
  if (dsum % 2 != 0) return false;

  for (int u = 0, x = n - 1; x >= 0; --x) {
    for (int i = 0; i < hist[x]; ++i) {
      d[u] = x;
      ++u;
    }
  }

  long long lcum = 0, rcum = 0;
  int p = n;
  for (long long k = 1; k <= n; ++k) {
    lcum += d[k - 1];
    if (p < k) {
      rcum -= d[p];
      ++p;
    }
    while (k <= p - 1 && d[p - 1] <= k) {
      --p;
      rcum += d[p];
    }
    if (lcum > k * (k - 1) + rcum + k * (p - k)) {
      return false;
    }
  }
  return true;
}

inline Graph havel_hakimi(std::vector<int> d) {
  assert(erdos_gallai(d));

  int n = d.size();
  int m = std::reduce(d.begin(), d.end(), 0) / 2;

  std::vector<int> p(n);
  {
    std::vector<int> hist(n);
    for (int x: d) ++hist[x];

    std::vector<int> pos(n);
    for (int x = n - 2; x >= 0; --x) {
      pos[x] = pos[x + 1] + hist[x + 1];
    }

    for (int u = 0; u < n; ++u) {
      p[pos[d[u]]] = u;
      ++pos[d[u]];
    }
    for (int i = 0, x = n - 1; x >= 0; --x) {
      for (int j = 0; j < hist[x]; ++j) {
        d[i] = x;
        ++i;
      }
    }
  }

  std::vector<int> last(n, -1);
  for (int i = 0; i < n; ++i) {
    last[d[i]] = i;
  }

  Graph G(n, m);
  for (int i = 0; i < n; ++i) {
    if (d[i] == 0) continue;

    int x = d[i], l = i, r = last[d[i]];
    while (true) {
      for (int j = r; j > l && x > 0; --j) {
        G.add_edge(p[i], p[j]);

        if (d[j - 1] == d[j]) {
          --last[d[j]];
        }
        else {
          last[d[j]] = -1;
        }
        --d[j];
        if (last[d[j]] == -1) {
          last[d[j]] = j;
        }
        --x;
      }
      if (x == 0) break;

      l = r;
      r = last[d[r + 1]];
    }

    if (last[d[i]] == i) last[d[i]] = -1;
    d[i] = 0;
  }
  return G;
}
