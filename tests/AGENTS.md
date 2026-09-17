# tests/ の作業指針

このファイルは `tests/` 以下に適用する。リポジトリルートの `README.md` にある開発方針・コーディング規約にも従う。

## テスト方針

- `sandbox/` と `thinking_memo/` を除くライブラリの各機能を対象とする。
- 外部テストフレームワークを導入せず、固定入力と `assert` を使う。
- 代表的な使用例と、空・1要素・重複・区間の端など、API の前提条件を満たす境界ケースを扱う。使用例としても読める短さを保つ。
- concept は `static_assert` で成立・不成立を確認する。
- 演算順序が重要な機能は非可換な演算でも確認する。複数の正解がある出力は妥当性を検証し、未規定の順序や実行ごとに変わるハッシュ値には依存しない。
- 本体の不具合が見つかったら、再現テストと原因を報告する。成功させるために期待値を誤った挙動へ合わせたり、テストを無効化したりしない。テスト作業の一環で本体を修正しない。

## 配置と記述

- 対象の配置に合わせて `tests/<directory>/<feature>.test.cpp` を置く。密接な concept や型エイリアスはまとめてよい。
- 各 `*.test.cpp` は独立した実行ファイルになるようにする。通常は `main()` を定義し、`template/main.hpp` 自体のテストでは同ヘッダの `main()` を使う。
- `run.sh` が `*.test.cpp` を自動検出するため、通常のテスト追加に実行対象の登録は不要。
- ルートの `.clang-format` に従う。`pretty` を検証する場合は、利用側のテンプレートより先に `template/debug.hpp` を include する。既存テストの `clang-format off/on` はこの順序を保つためのものなので維持する。
- 標準入出力をスコープ内で差し替える場合は `tests/support/io.hpp` の `TestIO` を使える。
- プロセスの入出力を検証する場合、`foo.test.cpp` に対して `foo.in`・`foo.out`・`foo.err` を置く。存在しないファイルは空として扱われ、出力は完全一致で比較される。`.out` は期待値ファイルなのでバージョン管理に含める。

## 実行と検証

以下はリポジトリルートで実行する。

```sh
# 全件
bash tests/run.sh

# コンパイラを明示
CXX=g++-15 bash tests/run.sh

# パスの部分一致で絞り込む（複数指定は OR）
bash tests/run.sh data_structure
bash tests/run.sh tests/data_structure/segment_tree.test.cpp
bash tests/run.sh mint fps
```

- GCC 15.2.0 と Bash を使用する。コンパイラは `CXX`、`g++-15`、`g++` の順で選ばれる。
- コンパイルオプションは `-std=gnu++23 -Wall -Wextra -Werror -O2 -UNDEBUG`。警告も失敗にし、`assert` は有効にする。
- 失敗後も残りを実行し、最後に成功・失敗件数を表示する。失敗がある場合と対象が見つからない場合は終了コードが非ゼロになる。
- 一時生成物は `tests/.build.*/` に作成され、終了時に削除される。生成物をコミットしない。
- テストコードを変更したら対象を実行する。実行スクリプトや共有ヘルパーを変更した場合、または広範囲にテストを追加・変更した場合は全件実行する。文書のみの変更では再実行は不要。

## コンパイル時の分岐と CI

- `template/io.test.cpp` と `template/template.test.cpp` は `LOCAL` の有無で実行する。
- `template/main.test.cpp` は `LOCAL` と `MULTI_TESTCASE` の全4通りで実行する。
- 追加実行の入出力ファイルには `.local`・`.multi`・`.multi_local` を付ける（例：`main.multi_local.err`）。各実行は専用ファイルを参照し、通常実行のファイルにはフォールバックしない。
- `main.multi_local.err` は ANSI エスケープ文字を含む。出力比較に必要なので保持する。
- 新たなコンパイル時の分岐を検証する場合は、`run.sh` の対象分岐と必要な入出力ファイルを追加する。
- CI は `.github/workflows/tests.yml` で定義され、push・pull request・手動実行時に `gcc:15.2.0` コンテナで `bash tests/run.sh` を実行する。ローカルと CI で同じテスト手順を維持する。
