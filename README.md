# cp-library
Collection of algorithms for competitive programming

Now migrating from https://github.com/fura2/competitive-programming-library

## 開発方針

コードを書く楽しみを奪われたくないので、ライブラリの主要部は原則として自分ですべての実装を書く。
LLM はライブラリ作成時の議論とテストコードの作成にのみ用いる。

## 設計方針

- コンパイラは g++ 15.2.0 を仮定する。
  - コンパイルオプション `-std=gnu++23 -Wall -Wextra -Wno-sign-compare -Wno-char-subscripts` のもとで、警告なしでコンパイルできることを保証する。
  - コンパイラに依存した機能は極力使わない。今のところ bits/stdc++.h のみが例外。
- a.cpp がコンテストで使うソースファイルのコピー元である。ここで include している template/template.hpp には競技プログラミング特有のイディオムがまとめられている。ライブラリの他の部分はこのヘッダを include せず、C++ 的にある程度きれいな書き方で実装されている。
- include はヘッダごとに必要十分な量を書く (template.hpp 以外)。
- クラス名と concept 名は CamelCase、関数名は snake_case とする。
- ライブラリ作成の際に考えたことは thinking_memo 以下にまとめている。
