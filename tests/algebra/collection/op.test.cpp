#include "algebra/collection/op.hpp"

#include <cassert>
#include <concepts>
#include <string>

template <typename T>
concept HasAdd = requires { op_add<T>; };
template <typename T>
concept HasMul = requires { op_mul<T>; };
template <typename T>
concept HasMin = requires { op_min<T>; };
template <typename T>
concept HasMax = requires { op_max<T>; };
template <typename T>
concept HasNeg = requires { op_neg<T>; };

struct Number {
  int value;
  friend Number operator+(const Number& a, const Number& b) {
    return {a.value + b.value};
  }
  friend Number operator*(const Number& a, const Number& b) {
    return {a.value * b.value};
  }
  friend bool operator<(const Number& a, const Number& b) {
    return a.value < b.value;
  }
  Number operator-() const { return {-value}; }
};

struct NoOperators {};

static_assert(HasAdd<Number> && HasMul<Number> && HasNeg<Number>);
static_assert(HasMin<Number> && HasMax<Number>);
// Typed standard functors alone do not reject unsupported operator bodies.
static_assert(!HasAdd<NoOperators> && !HasMul<NoOperators>);
static_assert(!HasMin<NoOperators> && !HasMax<NoOperators>);
static_assert(!HasNeg<NoOperators>);
static_assert(HasAdd<std::string> && !HasMul<std::string>);
static_assert(!HasNeg<std::string>);
// Arithmetic on short promotes to int, whereas selection still returns short.
static_assert(!HasAdd<short> && !HasMul<short> && !HasNeg<short>);
static_assert(HasMin<short> && HasMax<short>);

template <typename T>
void check() {
  const T a{3}, b{5};
  static_assert(std::same_as<decltype(op_add<T>(a, b)), T>);
  static_assert(std::same_as<decltype(op_mul<T>(a, b)), T>);
  static_assert(std::same_as<decltype(op_neg<T>(a)), T>);
  static_assert(std::same_as<decltype(op_min<T>(a, b)), T>);
  static_assert(std::same_as<decltype(op_max<T>(a, b)), T>);
  assert(op_add<T>(a, b) == 8 && op_mul<T>(a, b) == 15);
  assert(op_neg<T>(a) == -3);
  assert(op_min<T>(a, b) == 3 && op_min<T>(b, a) == 3);
  assert(op_max<T>(a, b) == 5 && op_max<T>(b, a) == 5);
  assert(op_min<T>(a, a) == 3 && op_max<T>(a, a) == 3);
}

int main() {
  check<int>();
  check<long long>();
  check<double>();
  const Number a{3}, b{5};
  assert(op_add<Number>(a, b).value == 8);
  assert(op_mul<Number>(a, b).value == 15);
  assert(op_neg<Number>(a).value == -3);
  assert(op_min<Number>(a, b).value == 3);
  assert(op_max<Number>(a, b).value == 5);
  const std::string text = "ab";
  static_assert(
      std::same_as<decltype(op_min<std::string>(text, text)), std::string>);
  assert(op_add<std::string>(text, "cd") == "abcd");
}
