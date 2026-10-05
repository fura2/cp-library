#pragma once

#include <cassert>
#include <chrono>
#include <ranges>
#include <vector>

class RollingHash {
  struct Value;

 public:
  using hash_type = Value;

  explicit RollingHash(): sz{0}, h1(1), h2(1) {}

  template <typename R>
    requires std::ranges::input_range<R> && std::ranges::sized_range<R>
  explicit RollingHash(R&& rg)
      : sz(std::ranges::size(rg)), h1(sz + 1), h2(sz + 1) {
    for (auto [i, e]: std::ranges::ref_view(rg) | std::views::enumerate) {
      h1[i + 1] = (h1[i] * base + e) % mod1;
      if (h1[i + 1] < 0) h1[i + 1] += mod1;
      h2[i + 1] = (h2[i] * base + e) % mod2;
      if (h2[i + 1] < 0) h2[i + 1] += mod2;
    }
  }

  int size() const { return sz; }

  hash_type hash() const { return hash_type{sz, h1[sz], h2[sz]}; }

  hash_type hash(int l, int r) const {
    assert(0 <= l && l <= r && r <= sz);
    allocate(r - l);
    auto res1 = (h1[r] - h1[l] * pow1[r - l]) % mod1;
    if (res1 < 0) res1 += mod1;
    auto res2 = (h2[r] - h2[l] * pow2[r - l]) % mod2;
    if (res2 < 0) res2 += mod2;
    return {r - l, res1, res2};
  }

  template <typename R>
    requires std::ranges::input_range<R> && std::ranges::sized_range<R>
  static hash_type hash(R&& rg) {
    long long res1 = 0, res2 = 0;
    int sz = std::ranges::size(rg);
    for (const auto& e: rg) {
      res1 = (res1 * base + e) % mod1;
      res2 = (res2 * base + e) % mod2;
    }
    if (res1 < 0) res1 += mod1;
    if (res2 < 0) res2 += mod2;
    return {sz, res1, res2};
  }

 private:
  static constexpr long long mod1 = 1e9 + 7, mod2 = 1e9 + 9;
  inline static const long long base =
      256 + std::chrono::steady_clock::now().time_since_epoch().count() %
                (mod1 - 256);
  inline static std::vector<long long> pow1 = {1}, pow2 = {1};
  int sz;
  std::vector<long long> h1, h2;

  struct Value {
    int sz;
    long long h1, h2;

    friend Value operator*(const Value& v1, const Value& v2) {
      auto [sz1, h11, h12] = v1;
      auto [sz2, h21, h22] = v2;

      RollingHash::allocate(sz2);
      long long res1 = (h11 * RollingHash::pow1[sz2] + h21) % RollingHash::mod1;
      long long res2 = (h12 * RollingHash::pow2[sz2] + h22) % RollingHash::mod2;
      return {sz1 + sz2, res1, res2};
    }

    friend bool operator==(const Value&, const Value&) = default;

    int size() const { return sz; }
  };

  static void allocate(int sz) {
    int k = pow1.size();
    if (k < sz + 1) {
      pow1.resize(sz + 1);
      pow2.resize(sz + 1);
      for (; k <= sz; ++k) {
        pow1[k] = pow1[k - 1] * base % mod1;
        pow2[k] = pow2[k - 1] * base % mod2;
      }
    }
  }
};
