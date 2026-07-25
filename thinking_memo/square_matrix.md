## 2026.5.17

`SquareMatrix& operator*=(const scalar_type& c)` は定義しない。
R が非可換の場合に、A = A * c と A = c * A のどちらの意味なのかはっきりしないため。

## 2026.7.25
FixedSquareMatrix に改名した。
また、MatrixSemiring のテンプレートパラメータを (数学の慣習に合うように) `<N, S>` の順にしたかったので、それに合わせてこちらも `<S, N>` から `<N, S>` に変更した。
標準ライブラリの `std::array<T, N>` とは揃わなくなってしまったが仕方ない。
