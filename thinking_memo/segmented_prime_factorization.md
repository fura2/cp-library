## 2026.9.21

オフセットが大きく比較的狭い区間 $[L,R]$ ($L,R\sim10^{14},R-L\sim10^7$ 程度) に含まれる整数を扱う文脈では、lpf 配列はあまり役に立たない。
$L\le x\le R$ に対して $\sqrt{R}\lt x/\textrm{lpf}(x)\lt L$ となりうるため。
そこで、$[L,R]$ 内の各整数 $x$ に対して、区間篩の構築過程で得られる $x$ の素因数分解を直接保持することにする。
この情報があれば、乗法的関数の値などが簡単に計算できる。

二種類のオーバーロード
```cpp
(1) auto segmented_prime_factorization(long long l, long long r)
(2) auto segmented_prime_factorization(long long l, long long r, const std::vector<int>& ps)
```
を用意した。
SegmentedSieve と同様、(2) の `ps` は昇順に並んだ素数のリストで、$\sqrt{r}$ 以下のすべての素数が重複なく現れることを仮定する。
