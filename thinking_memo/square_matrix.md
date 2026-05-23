## 2026.5.17

`SquareMatrix& operator*=(const scalar_type& c)` は定義しない。
R が非可換の場合に、A = A * c と A = c * A のどちらの意味なのかはっきりしないため。
