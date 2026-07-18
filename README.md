# cp-library
Collection of algorithms for competitive programming

Now migrating from https://github.com/fura2/competitive-programming-library

## 開発方針

プログラミングの楽しみを奪われたくないので、ライブラリの主要部は原則として自分ですべての実装を書く。
生成 AI は議論相手として、および、テストコードの作成にのみ用いる。

## 設計方針

- コンパイラは g++ 15.2.0 を仮定する。
  - コンパイルオプション `-std=gnu++23 -Wall -Wextra` のもとで、警告なしでコンパイルできることを保証する。
  - コンパイラに依存した機能は極力使わない。今のところ bits/stdc++.h のみが例外。
- a.cpp がコンテストで使うソースファイルのコピー元である。
  - ここで include している template/template.hpp には競技プログラミング特有のイディオムがまとめられている。ライブラリの他の部分はこのヘッダを include せず、C++ 的にある程度きれいな書き方で実装されている。
- ライブラリ作成の過程で考えたことは thinking_memo 以下にまとめる。

## コーディングスタイル

- [.clang-format](.clang-format) に基づいてフォーマットする。
- include はヘッダごとに必要十分な量を書く (template.hpp 以外)。
- クラス名とコンセプト名は PascalCase、関数名は snake_case とする。
- 1 引数のコンストラクタには explicit を指定する。ただし、以下の例外を含む。
  - mint のようなプリミティブな扱いをするデータ型には指定しない。
  - コピーコンストラクタ、ムーブコンストラクタには指定しない。
