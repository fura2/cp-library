## 2026.9.24

半群 $S$ に単位元を添加してモノイドを作る構成の実装。

`UnitizationOfSemigroup<S>::unwrap()` は、非単位元を保持していることを仮定し、`S::unwrap()` の値を返す。
単位元かどうかは `UnitizationOfSemigroup<S>::is_identity()` でチェックできる。

元々、`RightZeroSemigroup` に単位元を添加することで `LastMonoid` を作る構成をきれいに書きたくて作った。
しかし、この例では、`LastMonoid` を直接定義するのに比べて積の演算が有意に (ランダムケースで 10 倍程度) 遅くなってしまった。`LastMonoid` の積は
```cpp
[](const auto& a, const auto& b) { return b.has_value() ? b : a; }
```
のように書けて、これは `UnitizationOfSemigroup` の積に比べてコンパイラが最適化しやすい様子。

ある程度きれいには書けているので一応残しているが、今のところ使用例はない。
