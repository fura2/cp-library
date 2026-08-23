## 2026.8.17

$n\in\mathbb{Z}_{\ge0}$ に対して $\lfloor\sqrt n\rfloor$ を計算する関数 isqrt を実装した。

次のことを仮定する。これらは典型的な実行環境では成り立つものと期待できる。
- `long long` → `double` の変換は丸め方向 roundTiesToEven で行われる。つまり、表現可能な最も近い値に丸める。タイの場合は仮数部の最下位ビットが 0 となる方を採用する。
- `std::sqrt(double)` の計算結果は roundTiesToEven に対して correctly rounded (真の値を指定した丸め方向に丸めたもの) である。

これらの仮定のもとで、`static_cast<long long>(std::sqrt(static_cast<double>(n)))` が $\lfloor\sqrt n\rfloor$ か $\lfloor\sqrt n\rfloor+1$ のどちらかの値になることが証明できる[1]。

#### 直感的な理解

- 結果が $\lfloor\sqrt n\rfloor+1$ になる例
  - $n = 2^{54}-1$ のとき、`static_cast<double>(n)` $=2^{54}$ と丸められるので、`static_cast<long long>(std::sqrt(static_cast<double>(n)))` $=2^{27}=\lfloor\sqrt n\rfloor+1$。
- $n$ を `double` に変換したときに値が小さくなることもありうるが、その影響は `std::sqrt` の結果が丸められる際に消えてしまう程度に小さいため、結果が $\lfloor\sqrt n\rfloor-1$ になることはない。
- 元々 isqrt という正/負の方向について非対称なものを扱っているのだから、このような非対称な性質があってもそれほど不思議ではない。

#### コードの補足

$\lfloor\sqrt n\rfloor+1$ のケースではコード中の `r * r` がオーバフローしうるようにも見えるが、実際には `LLONG_MAX` $=2^{63}-1$ は次の平方数まで遠いので、この付近の $n$ に対しては $r=\lfloor\sqrt n\rfloor$ となっていて、`r * r` は long long の範囲内に収まる。

#### 参考文献
- [1] https://rsk0315.hatenablog.com/entry/2023/11/07/221428
