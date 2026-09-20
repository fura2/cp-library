# cp-library

[![Tests](https://github.com/fura2/cp-library/actions/workflows/tests.yml/badge.svg)](https://github.com/fura2/cp-library/actions/workflows/tests.yml)

Collection of algorithms for competitive programming

Now migrating from https://github.com/fura2/competitive-programming-library

## 開発方針

プログラミングの楽しみを奪われたくないので、ライブラリの主要部は原則として自分ですべての実装を書く。
生成 AI は議論相手として、および、テストコードの作成にのみ用いる。

## 設計方針

- コンパイラは g++ 15.2.0 を仮定する。
  - コンパイルオプション `-std=gnu++23 -Wall -Wextra -Werror` のもとでコンパイルできることを保証する。
  - コンパイラに依存した機能は極力使わない。今のところ bits/stdc++.h のみが例外。
- [x.cpp](x.cpp) がコンテストで使うソースファイルのコピー元である。
  - ここで include している [template/template.hpp](template/template.hpp) には競技プログラミング特有のイディオムがまとめられている。ライブラリの他の部分はこのヘッダを include せず、C++ 的にある程度きれいな書き方で実装されている。
- ライブラリ作成の過程で考えたことは [thinking_memo](thinking_memo) 以下にまとめる。

## コーディング規約

- [.clang-format](.clang-format) に基づいてフォーマットする。
- include はヘッダごとに必要十分な量を書く。template.hpp と main.hpp は例外。
- クラス名とコンセプト名は PascalCase、関数名は snake_case とする。
- 1 引数のコンストラクタには explicit を指定する。ただし、以下の例外を含む。
  - mint のようなプリミティブな扱いをするデータ型には指定しない。
  - コピーコンストラクタ、ムーブコンストラクタには指定しない。
  - `std::initializer_list` 型を引数とするコンストラクタには指定しない。
- `+` や `*` などの二項演算は friend 関数として定義する。
- requires は、`template<...>` がある宣言については前置、なければ後置で書く。
  - 可能な限り前置にしたいという気持ち。
- constexpr は原則として指定しない。
  - `INF` などの基本的な定数には指定する。
  - コンセプトの定義に使う定数や非型テンプレートパラメータ (NTTP) に渡すラムダ式など、定数式であることが必要な場合には指定する。
  - 競技プログラミングでは定数式の評価はほとんどしないので、見た目をすっきりさせて認知負荷を減らすことを優先している。
- size_t は可能な限り使わない。
  - C++ の標準ライブラリに合わせるより、気軽に符号つき整数型を扱えることの方が大切。
- 関数の戻り値の型が複雑な場合には、積極的に auto による型推論を使う。
