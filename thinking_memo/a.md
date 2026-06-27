## 2026.5.31

マルチテストケース用のコードを追加。
```cpp
#define MULTI_TESTCASE
```
のコメントアウトを解除することでマルチテストケースに対応する仕様にした。

## 2026.6.14

フォーマット設定を[ライブラリ用](../.clang-format)と[本番用](https://github.com/fura2/cp-utils/blob/main/.clang-format)で分けることにした。
main.cpp だけは、本番用の設定に従うべきであって、ライブラリ用に従っていなくてもよいものとした。

## 2026.6.27

最小限だけを残して、main 関数なども全部 template.hpp に写した。
コンテスト中は testcase 関数だけを実装する形。
それに伴い、ファイル名を main.cpp から a.cpp に変更した。
