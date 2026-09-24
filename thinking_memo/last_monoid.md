## 2026.9.24

値を代入する作用を表すのに使うモノイド。最も右にある非単位元を選ぶ。

双対版である「最も左にある非単位元を選ぶ」ような演算を入れたものは `FirstMonoid` と呼んでいる。
これらは必ずしも一般的な名称ではなく、アカデミックでも定着している名前は無さそう。Haskell で同じものが `Data.Monoid.First`, `Data.Monoid.Last` と命名されていたのでそれに倣った。

このモノイドは right zero semigroup に単位元を添加して得られる。
そのような構成は UnitizationOfSemigroup として実装しているが、[unitization_of_semigroup.md](unitization_of_semigroup.md) に書いたように速度上の懸念があったので、今回は独立に実装した。
