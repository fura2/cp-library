## 2026.5.17

`SquareMatrix& operator*=(const scalar_type& c)` は定義しない。
S が非可換の場合に、A = A * c と A = c * A のどちらの意味なのかはっきりしないため。

## 2026.7.25

FixedSquareMatrix に改名した。
また、MatrixSemiring のテンプレートパラメータを (数学の慣習に合うように) `<N, S>` の順にしたかったので、それに合わせてこちらも `<S, N>` から `<N, S>` に変更した。
標準ライブラリの `std::array<T, N>` とは揃わなくなってしまったが仕方ない。

## 2026.7.26

初期化方法についてメモ。
```cpp
FixedSquareMatrix<2, R> A{{{0, 1}, {2, 3}}};
FixedSquareMatrix<2, R> B = {{{0, 1}, {2, 3}}};
```
のように初期化できる。直感より一段多くの波括弧が必要なことに注意。これら 2 行はどちらも
```cpp
constexpr FixedSquareMatrix(const S (&a)[N][N])
```
とマッチしている。このコンストラクタ無しで
```cpp
constexpr explicit FixedSquareMatrix(matrix_type&& mat)
```
(から explicit を消したもの) とマッチさせようとすると、C++ の微妙な仕様との兼ね合いで、`A{{{{0, 1}, {2, 3}}}}` のように波括弧をさらに一段挟む必要がある。
