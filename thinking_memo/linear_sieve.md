## 2026.9.4

線形篩の実装。

$a$ の約数列挙では、約数を列挙して $\texttt{ds}$ に格納したあと全体をソートしているので、計算量が $O(d(a)\log d(a))$ になっている。$a$ の因子 $p^e$ ごとに $\texttt{ds},p\texttt{ds},\cdots,p^e\texttt{ds}$ をマージソートすれば $O(d(a)\log\log a)$ にできる。そちらは実装はしていない。

#### 参考文献
- https://cp-algorithms.com/algebra/prime-sieve-linear.html
