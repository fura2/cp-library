## 2026.9.25

一般のモノイド作用の上の遅延セグメント木の実装。

とりあえず現行で使っている再帰版を移植した。非再帰による高速化や二分探索などは未実装。

## 2026.10.3

非再帰版を学んで実装した。意外なことに再帰版よりシンプルに感じた。学習の際は [1][2][3] が参考になった。

#### 実装メモ
- 実装全体を通しての重要な不変条件 : `fold` や `apply` が呼び出される時点で、`x[u]` には `f[u]` を適用した後の値が入っている。
  - [1] の We assume that `t[i]` already includes `d[i]` に相当。
- メンバ関数名
  - `fix(u)` : `u` の親から根までの各ノード `v` に対して、ボトムアップに、`x[v]` の値を再計算して正常な値に直す。update と呼ばれることもある。
  - `push(u)` : 根から `u` の親までの各ノード `v` に対して、トップダウンに、遅延していた `f[v]` を子に伝播させる。propagate と呼ばれることもある。
  - `calc(u, g)` : `x[u]` に `g` を適用する。ACL では `all_apply` と呼ばれている。

#### 参考文献
- [1] Al.Cash, [Efficient and easy segment trees](https://codeforces.com/blog/entry/18051)
- [2] yaketake08, [非再帰版の遅延評価セグメント木の実装メモ](https://smijake3.hatenablog.com/entry/2018/11/03/100133)
- [3] maspy, [Segment Tree のお勉強(2)](https://maspypy.com/segment-tree-%e3%81%ae%e3%81%8a%e5%8b%89%e5%bc%b72)
