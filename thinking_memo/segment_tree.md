## 2026.8.12

モノイド上 $M$ の segment tree を実装した。
- FenwickTree クラスで用意した `apply(i, v)` は実装しないことにした。そちらでは $M$ が可換モノイドであることを仮定している一方、こちらは M は非可換でもよいので、左右どちらから `v` をかけるのかが曖昧になってしまうため。

## 2026.8.13

max_right と min_left を実装した。
すぐ忘れそうなので max_right のコードの意図をメモしておく。
```cpp
  auto i = sz + l;
  M cum = M::identity();
  while (true) {
    while ((i & 1) == 0) i >>= 1;

    if (!f(cum * a[i])) {
      while (i < sz) {
        i <<= 1;
        if (f(cum * a[i])) {
          cum = cum * a[i];
          ++i;
        }
      }
      return i - sz;
    }

    cum = cum * a[i];

    if (std::has_single_bit(i + 1)) break;
    ++i;
  }
  return n;
```
基本的なアイデアは、上がって下がる。以下では、セグメント木の入力となるモノイドの元の列を区間 $[x,y)$ 上で積をとったものを $A[x,y)$ と書く。
1. $[l, l+1)$ に対応する葉ノードから始めて、上 or 右に移動しながら、
    ```math
    f(A[l,x+d))=f(A[l,x)\cdot A[x, x+d))=\text{false}
    ```
    なる区間 $[x, x+d)$ に対応するノードを見つける。ここで、$f(A[l,x))=\text{true}$ である。
    $f(A[l, n))=\text{true}$ のときは、そのようなノードが存在しないので $n$ を返す。
1. 見つかった $[x, x+d)$ のノードから始めて、$f$ の値を見ながら左右どちらかの子に降りていく。

#### コードの各部分の簡単な説明

ノード $i$ が左の子でなくなるまで、親に移動する。
```cpp
    while ((i & 1) == 0) i >>= 1;
```

$f(A[l,x+d))=\text{false}$ なるノード $i$ が見つかった。このとき、$\text{cum} = A[l,x)$ であり、ノード $i$ は区間 $[x,x+d)$ に対応している。手順 2 に進む。
```cpp
    if (!f(cum * a[i])) {
      ...
    }
```

ノード $i$ は右の子でかつ $f(A[l,x+d))=\text{true}$ となっている。ノード $i$ からの寄与 $A[x,x+d)$ を $\text{cum}$ に含めて、一つ右のノード $i+1$ に移動する。このノードは区間 $[x+d,x+2d)$ に対応している。
ノード $i$ が既に (その高さの中で) 最も右のノードなら、$f(A[l, n))=\text{true}$ のケースなので、while を抜けて $n$ を返す。
```cpp
    cum = cum * a[i];
    if (std::has_single_bit(i + 1)) break;
    ++i;
```
