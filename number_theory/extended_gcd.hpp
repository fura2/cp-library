#pragma once

#include <cmath>
#include <tuple>

template <typename T>
std::tuple<T, T, T> extended_gcd(T a, T b) {
  if (a == 0) return {std::abs(b), 0, b > 0 ? 1 : b < 0 ? -1 : 0};
  if (b == 0) return {std::abs(a), a > 0 ? 1 : a < 0 ? -1 : 0, 0};

  bool aneg = false, bneg = false;
  if (a < 0) aneg = true, a *= -1;
  if (b < 0) bneg = true, b *= -1;

  T s = a, xs = 1, ys = 0;
  T t = b, xt = 0, yt = 1;
  while (t > 0) {
    T k = s / t;
    T u = s - k * t, xu = xs - k * xt, yu = ys - k * yt;
    s = t, xs = xt, ys = yt;
    t = u, xt = xu, yt = yu;
  }

  if (aneg) a *= -1, xs *= -1;
  if (bneg) b *= -1, ys *= -1;
  if (xs < 0) {
    if (b > 0) {
      xs += b / s;
      ys -= a / s;
    }
    else {
      xs -= b / s;
      ys += a / s;
    }
  }

  return {s, xs, ys};
}
