## 2026.9.4

線形篩の実装。

`lpf` 配列を使った約数列挙にも対応している。$a$ の約数列挙では、約数を列挙して $\texttt{ds}$ に格納したあと全体をソートしているので、計算量が $O(d(a)\log d(a))$ になっている。$a$ の因子 $p^e$ ごとに $\texttt{ds},p\texttt{ds},\cdots,p^e\texttt{ds}$ をマージソートすれば $O(d(a)\log\log a)$ にできるが、そちらは実装はしていない。

## 2026.9.20

LinearSieve クラスには篩としての機能のみを残し、`lpf` 配列を使ってできる様々な計算 (cf. [2] の 5 節) はこのクラスに含めない。
それらは独立した関数として `lpf` を引数で与える形式で用意する。

$\texttt{lpf}[0] = \texttt{lpf}[1] = -1$ を仕様とする。こうしておけば、$x$ が素数かどうかが `lpf[x] == x` で判定できるようになる。

#### 参考文献
- [1] https://cp-algorithms.com/algebra/prime-sieve-linear.html
- [2] https://info.atcoder.jp/entry/algorithm_lectures/prime_sieve
