#include "number_theory/multiplicative_function.hpp"

#include <cassert>
#include <concepts>
#include <vector>

#include "mint/mint.hpp"
#include "number_theory/linear_sieve.hpp"

template <typename F>
concept CanEvaluateMultiplicativeFunction = requires(
    F f, const std::vector<int>& lpf) { multiplicative_function(1, f, lpf); };

template <typename F>
concept CanBuildMultiplicativeFunctionTable =
    requires(F f, const std::vector<int>& lpf) {
      multiplicative_function_table(f, lpf);
    };

int main() {
  auto tau = [](int, int e) { return e + 1; };
  auto sigma = [](int p, int e) {
    long long result = 1, power = 1;
    for (int i = 0; i < e; ++i) {
      power *= p;
      result += power;
    }
    return result;
  };
  auto mu = [](int, int e) { return e == 1 ? -1 : 0; };

  static_assert(CanEvaluateMultiplicativeFunction<decltype(tau)>);
  static_assert(CanBuildMultiplicativeFunctionTable<decltype(tau)>);
  static_assert(!CanEvaluateMultiplicativeFunction<int>);
  static_assert(!CanBuildMultiplicativeFunctionTable<int>);
  using Unary = decltype([](int e) { return e + 1; });
  static_assert(!CanEvaluateMultiplicativeFunction<Unary>);
  static_assert(!CanBuildMultiplicativeFunctionTable<Unary>);

  // f(1) = 1 without evaluating any prime power.
  const std::vector<int> one = {-1, -1};
  int calls = 0;
  auto counted = [&calls](int, int) {
    ++calls;
    return 7;
  };
  assert(multiplicative_function(1, counted, one) == 1);
  auto one_table = multiplicative_function_table(counted, one);
  assert(one_table.size() == 2 && one_table[1] == 1);
  assert(calls == 0);

  const LinearSieve sieve{60};
  const auto& lpf = sieve.least_prime_factors();
  auto tau_table = multiplicative_function_table(tau, lpf);
  auto sigma_table = multiplicative_function_table(sigma, lpf);
  static_assert(
      std::same_as<decltype(multiplicative_function(1, tau, lpf)), int>);
  static_assert(std::same_as<decltype(multiplicative_function(1, sigma, lpf)),
                             long long>);
  static_assert(std::same_as<decltype(tau_table), std::vector<int>>);
  static_assert(std::same_as<decltype(sigma_table), std::vector<long long>>);
  assert(tau_table.size() == 61 && sigma_table.size() == 61);
  for (int a = 1; a <= 60; ++a) {
    int count = 0;
    long long sum = 0;
    for (int d = 1; d <= a; ++d) {
      if (a % d == 0) {
        ++count;
        sum += d;
      }
    }
    assert(multiplicative_function(a, tau, lpf) == count);
    assert(tau_table[a] == count);
    assert(multiplicative_function(a, sigma, lpf) == sum);
    assert(sigma_table[a] == sum);
  }

  // Primes, prime powers, and products of two or three distinct primes.
  const std::vector<int> expected_mu = {0,  1,  -1, -1, 0, -1, 1,  -1, 0,  0, 1,
                                        -1, 0,  -1, 1,  1, 0,  -1, 0,  -1, 0, 1,
                                        1,  -1, 0,  0,  1, 0,  0,  -1, -1};
  auto mu_table = multiplicative_function_table(mu, lpf);
  assert(mu_table.size() == 61);
  for (int a = 1; a <= 30; ++a) {
    assert(multiplicative_function(a, mu, lpf) == expected_mu[a]);
    assert(mu_table[a] == expected_mu[a]);
  }

  // Preserve wide integer and modular return types, including large products.
  auto sixth_power = [](int p, int e) {
    long long result = 1;
    for (int i = 0; i < 6 * e; ++i) result *= p;
    return result;
  };
  auto modular_power = [sixth_power](int p, int e) {
    return mint{sixth_power(p, e)};
  };
  auto power_table = multiplicative_function_table(sixth_power, lpf);
  auto modular_table = multiplicative_function_table(modular_power, lpf);
  static_assert(
      std::same_as<decltype(multiplicative_function(1, modular_power, lpf)),
                   mint>);
  static_assert(std::same_as<decltype(modular_table), std::vector<mint>>);
  assert(power_table.size() == 61 && modular_table.size() == 61);
  for (int a = 1; a <= 60; ++a) {
    long long expected = 1LL * a * a * a * a * a * a;
    assert(multiplicative_function(a, sixth_power, lpf) == expected);
    assert(power_table[a] == expected);
    assert(multiplicative_function(a, modular_power, lpf).unwrap() ==
           expected % 998244353);
    assert(modular_table[a].unwrap() == expected % 998244353);
  }

  // The LPF array need not come from LinearSieve.
  const std::vector<int> supplied = {-1, -1, 2, 3, 2, 5, 2, 7, 2, 3, 2, 11, 2};
  assert(multiplicative_function(12, sigma, supplied) == 28);
  auto supplied_table = multiplicative_function_table(sigma, supplied);
  assert(supplied_table.size() == 13 && supplied_table[12] == 28);
}
