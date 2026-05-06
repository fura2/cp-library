# cp-library
Collection of algorithms for competitive programming

Now migrating from https://github.com/fura2/competitive-programming-library

## 開発方針

このリポジトリは、個人の学習やアルゴリズムを自力で集めて鑑賞する目的も兼ねている。したがって、一部の例外を除いてコードは基本的に自分で書き、また、競技プログラミングではあまり使われないようなアルゴリズムも含む可能性がある。
- ライブラリ作成時の議論とテストコードの作成には、積極的に AI エージェントを使う。

## 設計方針

- コンパイラは g++ 15.2.0 を仮定する。
  - コンパイルオプション `-std=gnu++23 -Wall -Wextra -Wno-sign-compare -Wno-char-subscripts` のもとで、警告なしでコンパイルできることを保証する。
  - コンパイラに依存した機能は極力使わない。今のところ bits/stdc++.h のみが例外。
- コンテストで使う際の起点となるヘッダファイルは template/template.hpp である。`using namespace std;` や rep マクロなどの競技プログラミング特有の構文はこのファイルにのみ現れる。ライブラリの他の部分はこのファイルを include せず、C++ 的にある程度きれいな書き方で実装されている。
- ライブラリ作成の際に考えたことは thinking_memo 以下にまとめている。
- include はヘッダごとに必要十分な量を書く (template.hpp 以外)。
- クラス名と concept 名は CamelCase、関数名は snake_case とする。
