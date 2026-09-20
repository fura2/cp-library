## 2026.9.20

二種類のコンストラクタ
```cpp
(1) SegmentedSieve(long long l, long long r): l{l}, r{r}
(2) SegmentedSieve(long long l, long long r, const std::vector<int>& ps)
```
を用意した。
- (1) は単に閉区間 $[l,r]$ を篩う。
- (2) は小さい素数のリスト `ps` を与えた上で $[l,r]$ を篩う。ここで、`ps` は昇順に並んだ素数のリストで、$\sqrt{r}$ 以下のすべての素数が含まれていることを仮定する。様々な $[l,r]$ に対して `ps` を使い回すような状況で使うことを想定している。
