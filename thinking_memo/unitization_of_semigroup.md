## 2026.9.22

半群 $S$ に単位元を添加してモノイドを作る構成の実装。

`UnitizationOfSemigroup<S>::unwrap()` は、非単位元を保持していることを仮定し、`S::unwrap()` の値を返す。
単位元かどうかは `UnitizationOfSemigroup<S>::is_identity()` でチェックできる。
