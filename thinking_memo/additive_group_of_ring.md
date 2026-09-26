## 2026.9.26

Ring から乗法を忘れることで Group を作る構成の実装。
元々は `IntAddGroup`, `LintAddGroup`, `DoubleAddGroup` を実装するのに使っていたが、一般の `AddGroup<T, Zero>` を定義することにしたので、今は使用例はない。
