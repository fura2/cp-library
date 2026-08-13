## 2026.8.14

MonoidImpl を使って DoubleMinMonoid を作る際に、些細な実装の違いによって有意な速度差が観測されたのでメモしておく。ABC469 E の復習中に発見。

以下の計測結果は `SegmentTree<DoubleMinMonoid>` の `set` と `fold` を繰り返し呼んだときのもの。コンパイラは g++ 15.2.0 で -O2 オプションをつけている。

MonoidImpl の Op として次の 3 種類を考える。
```cpp
(1) [](double a, double b) { return std::min(a, b); }
(2) [](const double& a, const double& b) { return std::min(a, b); }
(3) [](double a, double b) { return b < a ? b : a; }
```
(1) を基準として、(2) は **1.6 倍**、(3) は **3.3 倍**速くなった。
`std::min` のシグネチャは
```cpp
template <class T>
constexpr const T& min(const T& a, const T& b);
```
となっている。参照を引数にとって参照を返すという仕様との兼ね合いで、どうやら (1) や (2) ではコンパイラの最適化が完全には効かないらしい。
(1) が (2) より遅いのは、(1) の場合には参照を `std::min` に渡すために一度メモリ上に a と b の値を展開する手順が挟まるため、という雰囲気ではないかと想像している。

モノイドや他の代数系の具体的な定義では、`std::min`, `std::max` は極力使わず、三項演算子で済ませる方針にする。
