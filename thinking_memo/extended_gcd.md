## 2026.9.18

$a,b\in\mathbb{Z}$ に対して、`extended_gcd(a, b)` は $ax+by=\mathrm{gcd}(a,b)$ を満たす三つ組 $(g=\mathrm{gcd}(a,b),x,y)$ を計算する。
ここで、$a,b$ が負であっても $\mathrm{gcd}(a,b)\ge0$ であり、また、$\mathrm{gcd}(0,0)=0$ と定める。

一般には $x,y$ の選び方には任意性があるが、次のようにして結果を一意に定めている。
- $b=0$ のとき、$x=\mathrm{sgn}(a),y=0$ と選ぶ。
- $b\ne0$ のとき、$0\le x\lt|b|/g$ を満たす唯一の $x,y$ を取る。
  - $a=0$ のとき $y=\mathrm{sgn}(b)$、$a\ne0$ のとき $|y|\le|a|/g$ が自動的に満たされる。
